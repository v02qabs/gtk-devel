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

#define BUF_LEN 256             /* バッファのサイズ */

int debug_flg = 0;      /* -d オプションを付けると turn on する */


/*--------------------------------------------------
 * ソケットから1行読み込む
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
 * レスポンスを取得する。^\d\d\d- ならもう1行取得
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
 * 指定されたソケット socket に文字列 p を送信。
 * 文字列 p の終端は \0 で terminate されている
 * 必要がある
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
    int command_socket;           /* コマンド用ソケット */
    int data_socket;              /* データ用ソケット */
    int data_waiting_socket;          /* データコネクションの待ち受け用ソケット */
    struct hostent *servhost;         /* ホスト名とIPアドレスを扱うための構造体 */
    struct sockaddr_in server;        /* ソケットを扱うための構造体 */
    struct sockaddr_in sin;
    int len;

    char send_mesg[BUF_LEN];          /* サーバに送るメッセージ */
    char user[BUF_LEN];           /* ftp サーバに送信するユーザ名 */
    char passwd[BUF_LEN];             /* ftp サーバに送信するパスワード */
    char host[BUF_LEN];               /* 接続するホスト名 */
    char path[BUF_LEN];               /* 要求するパス */
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
                /* 引数解析 */
    if ( argc == 5 ){
        strncpy(user,   argv[1], sizeof(user));
        strncpy(passwd, argv[2], sizeof(passwd));
        strncpy(host,   argv[3], sizeof(host));
        strncpy(path,   argv[4], sizeof(path));
    } else {
        fprintf(stderr, "ftp-client ユーザ名 パスワード ホスト名 パス\n");
        exit(1);
    }
                                /* ホストの情報 (IP アドレスなど) を取得 */
    servhost = gethostbyname(host);
    if ( servhost == NULL ){
        fprintf(stderr, "Bad hostname [%s]\n", host);
        exit(1);
    }

                                /* IP アドレスを示す構造体をコピー */
    bzero((char*)&server, sizeof(server));
    server.sin_family = AF_INET;
                /* 構造体をゼロクリア */
    bcopy(servhost->h_addr, (char *)&server.sin_addr, servhost->h_length);

                /* ポート番号取得 */
    server.sin_port = (getservbyname("ftp", "tcp"))->s_port;

                /* ソケット生成 */
    command_socket = socket(AF_INET, SOCK_STREAM, 0);

                /* サーバに接続 */
    connect(command_socket, (struct sockaddr *)&server, sizeof(server));

                /* welcome response を取得 */
    read_response(command_socket, buf);

                /* USER・PASS を送信 */
    sprintf(send_mesg, "USER %s\n", user);
    write_to_server(command_socket, send_mesg);
    read_response(command_socket, buf);

    sprintf(send_mesg, "PASS %s\n", passwd);
    write_to_server(command_socket, send_mesg);
    read_response(command_socket, buf);

                /* データコネクション用ソケットを作成し、
                 * bind・listen する
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
    /* まだ accept はしない。PORT・LIST を送ってから */


    /* ----------------------------------------- */
    {
        u_long local_ip;

        /* localhost の IP アドレスを取得。既に ESTABLISHED である
         * command_socket から取得していることに注意。
         */

        len = sizeof(sin);
        if ( getsockname(command_socket,
                         (struct sockaddr *)&sin, &len) < 0 ){
            error("getsockname failed.\n");
        }
        local_ip = ntohl(sin.sin_addr.s_addr);

                /* ポート番号を取得 */
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
                 * ↑は inet_ntoa(local_ip) でもいいんだけど、
                 * その場合はピリオドをカンマに変換しないといけない。
                 */
                (ntohs(sin.sin_port) >>  8) & 0xff,
                 ntohs(sin.sin_port)        & 0xff);

                /* PORT・RETR を送信 */
        write_to_server(command_socket, send_mesg);
        read_response(command_socket, buf);

        sprintf(send_mesg, "RETR %s\n", path);
        write_to_server(command_socket, send_mesg);
    }

                /* データコネクションの確立 */
    len = sizeof(sin);
    data_socket = accept(data_waiting_socket, (struct sockaddr *)&sin, &len);
    if ( data_socket == -1 ){
        error("accept failed.\n");
    }

                /* ファイルの内容取得 */
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
                 * のようなレスポンスを受け取る
                 */
    read_response(command_socket, buf);
                /* 226 Transfer complete. のようなレスポンスを受け取る */
    read_response(command_socket, buf);

                /* QUIT 送って終了 */
    write_to_server(command_socket, "QUIT\n");
    read_response(command_socket, buf);

    close(data_waiting_socket);
    close(command_socket);

    return 0;
}
