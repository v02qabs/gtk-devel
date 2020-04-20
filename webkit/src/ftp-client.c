/* $Id: ftp-client.c,v 1.4 2004/05/29 05:36:31 68user Exp $ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <unistd.h>
#include <ctype.h>

#define BUF_LEN 256             /* �Хåե��Υ����� */

int debug_flg = 0;      /* -d ���ץ������դ���� turn on ���� */


/*--------------------------------------------------
 * �����åȤ���1���ɤ߹���
 */
char *read_line(int socket, char *p){
    char *org_p = p;

    while (1){
        if ( read(socket, p, 1) == 0 ) break;
        if ( *p == '\n' ) break;
        p++;
    }
    *(++p) = '\0';
    return org_p;
}


/*--------------------------------------------------
 * �쥹�ݥ󥹤�������롣^\d\d\d- �ʤ�⤦1�Լ���
 */
void read_response(int socket, char *p){
    do { 
        read_line(socket, p);
        if ( debug_flg ){
            fprintf(stderr, "<-- %s", p);
        }
    } while ( isdigit(p[0]) &&
              isdigit(p[1]) && 
              isdigit(p[2]) &&
              p[3]=='-' );
}


/*--------------------------------------------------
 * ���ꤵ�줿�����å� socket ��ʸ���� p ��������
 * ʸ���� p �ν�ü�� \0 �� terminate ����Ƥ���
 * ɬ�פ�����
 */

void write_to_server(int socket, char *p){
    if ( debug_flg ){
        fprintf(stderr, "--> %s", p);
    }
    write(socket, p, strlen(p));
}

void error( char *message ){
    fprintf(stderr, message);
    exit(1);
}


int main(int argc, char *argv[]){
    int command_socket;           /* ���ޥ���ѥ����å� */
    int data_socket;              /* �ǡ����ѥ����å� */
    int data_waiting_socket;          /* �ǡ������ͥ��������Ԥ������ѥ����å� */
    struct hostent *servhost;         /* �ۥ���̾��IP���ɥ쥹�򰷤�����ι�¤�� */
    struct sockaddr_in server;        /* �����åȤ򰷤�����ι�¤�� */
    struct sockaddr_in sin;
    int len;

    char send_mesg[BUF_LEN];          /* �����Ф������å����� */
    char user[BUF_LEN];           /* ftp �����Ф���������桼��̾ */
    char passwd[BUF_LEN];             /* ftp �����Ф���������ѥ���� */
    char host[BUF_LEN];               /* ��³����ۥ���̾ */
    char path[BUF_LEN];               /* �׵᤹��ѥ� */
    char buf[BUF_LEN];

    while (1){
        int c;
        c = getopt(argc, argv, "d");
        if ( c == -1 ) break;
        switch (c){
          case 'd':
            debug_flg = 1;
            argc--;
            argv++;
            break;
          default:
            break;
        }
    }
                /* �������� */
    if ( argc == 5 ){
        strncpy(user,   argv[1], sizeof(user));
        strncpy(passwd, argv[2], sizeof(passwd));
        strncpy(host,   argv[3], sizeof(host));
        strncpy(path,   argv[4], sizeof(path));
    } else {
        fprintf(stderr, "ftp-client �桼��̾ �ѥ���� �ۥ���̾ �ѥ�\n");
        exit(1);
    }
                                /* �ۥ��Ȥξ��� (IP ���ɥ쥹�ʤ�) ����� */
    servhost = gethostbyname(host);
    if ( servhost == NULL ){
        fprintf(stderr, "Bad hostname [%s]\n", host);
        exit(1);
    }

                                /* IP ���ɥ쥹�򼨤���¤�Τ򥳥ԡ� */
    bzero((char*)&server, sizeof(server));
    server.sin_family = AF_INET;
                /* ��¤�Τ򥼥��ꥢ */
    bcopy(servhost->h_addr, (char *)&server.sin_addr, servhost->h_length);

                /* �ݡ����ֹ���� */
    server.sin_port = (getservbyname("ftp", "tcp"))->s_port;

                /* �����å����� */
    command_socket = socket(AF_INET, SOCK_STREAM, 0);

                /* �����Ф���³ */
    connect(command_socket, (struct sockaddr *)&server, sizeof(server));

                /* welcome response ����� */
    read_response(command_socket, buf);

                /* USER��PASS ������ */
    sprintf(send_mesg, "USER %s\n", user);
    write_to_server(command_socket, send_mesg);
    read_response(command_socket, buf);

    sprintf(send_mesg, "PASS %s\n", passwd);
    write_to_server(command_socket, send_mesg);
    read_response(command_socket, buf);

                /* �ǡ������ͥ�������ѥ����åȤ��������
                 * bind��listen ����
                 */
    data_waiting_socket = socket(AF_INET, SOCK_STREAM, 0);

    sin.sin_family = AF_INET;
    sin.sin_port = 0;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);

    if ( bind(data_waiting_socket, (struct sockaddr *)&sin, sizeof sin) < 0 ){
        error("bind failed.\n");
    }
    if ( listen(data_waiting_socket, SOMAXCONN) == -1 ){
        error("listen failed.\n");
    }
    /* �ޤ� accept �Ϥ��ʤ���PORT��LIST �����äƤ��� */


    /* ----------------------------------------- */
    {
        u_long local_ip;

        /* localhost �� IP ���ɥ쥹����������� ESTABLISHED �Ǥ���
         * command_socket ����������Ƥ��뤳�Ȥ���ա�
         */

        len = sizeof(sin);
        if ( getsockname(command_socket,
                         (struct sockaddr *)&sin, &len) < 0 ){
            error("getsockname failed.\n");
        }
        local_ip = ntohl(sin.sin_addr.s_addr);

                /* �ݡ����ֹ����� */
        if ( getsockname(data_waiting_socket,
                         (struct sockaddr *)&sin, &len) < 0 ){
            error("getsockname failed.\n");
        }

        sprintf(send_mesg, "PORT %d,%d,%d,%d,%d,%d\n",
                (int)(local_ip >> 24) & 0xff,
                (int)(local_ip >> 16) & 0xff,
                (int)(local_ip >>  8) & 0xff,
                (int)(local_ip)       & 0xff,
                /*
                 * ���� inet_ntoa(local_ip) �Ǥ⤤��������ɡ�
                 * ���ξ��ϥԥꥪ�ɤ򥫥�ޤ��Ѵ����ʤ��Ȥ����ʤ���
                 */
                (ntohs(sin.sin_port) >>  8) & 0xff,
                 ntohs(sin.sin_port)        & 0xff);

                /* PORT��RETR ������ */
        write_to_server(command_socket, send_mesg);
        read_response(command_socket, buf);

        sprintf(send_mesg, "RETR %s\n", path);
        write_to_server(command_socket, send_mesg);
    }

                /* �ǡ������ͥ������γ�Ω */
    len = sizeof(sin);
    data_socket = accept(data_waiting_socket, (struct sockaddr *)&sin, &len);
    if ( data_socket == -1 ){
        error("accept failed.\n");
    }

                /* �ե���������Ƽ��� */
    while (1){
        int read_size;
        read_size = read(data_socket, buf, BUF_LEN);
        if ( read_size > 0 ){
            write(1, buf, read_size);
        } else {
            break;
        }
    }
                /* 150 Opening ASCII mode data connection ... 
                 * �Τ褦�ʥ쥹�ݥ󥹤�������
                 */
    read_response(command_socket, buf);
                /* 226 Transfer complete. �Τ褦�ʥ쥹�ݥ󥹤������� */
    read_response(command_socket, buf);

                /* QUIT ���äƽ�λ */
    write_to_server(command_socket, "QUIT\n");
    read_response(command_socket, buf);

    close(data_waiting_socket);
    close(command_socket);

    return 0;
}
