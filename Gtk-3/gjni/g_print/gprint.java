public class gprint
{
	static
	{
		System.loadLibrary("gprint");
	}
	public native void show(String message);
	public static void main(String[] args)
	{
		gprint gp = new gprint();
		gp.show("Hello.");
	}
}

