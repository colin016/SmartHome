package com.zju.shp;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.Set;

public class ShpConnection {

	public static String 		IP 		= "127.0.0.1";//"192.168.0.232";//"10.239.204.168";// "192.168.2.3";
	public static final int	PORT	= 9596;
	private static Socket 		soc  		= null;
	private SocketChannel 		soc_channel = null;
	private boolean  			thread_close_condition 	= false;
	private ByteBuffer recvBuf = ByteBuffer.allocate(4096);
	private ByteBuffer sendBuf = ByteBuffer.allocate(1024);
	public ShpConnection()
	{
		SocThread thread = new SocThread();
		thread.start();
	}
	
	private void sendData(SocketChannel socketChannel, byte[] bytes) throws IOException 
	{

	} 
	
	public void sendData(ByteBuffer bytes) throws IOException
	{
		try {
			if(soc_channel != null && soc_channel.isConnected()){
				soc_channel.write(bytes);
			}else{
				System.out.println("soc_channel.isConnected == flase");
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			thread_close_condition = true;
			e.printStackTrace();
		}
	}
	class SocThread extends Thread{
		public void run() {
			Selector selector;
			while(true){
				System.out.println("SocThread running...");
				try {
					thread_close_condition = false;

					selector = Selector.open();
					// ����һ���׽���ͨ����ע���������ʹ���޲���ʽ
					soc_channel = SocketChannel.open();
					// ����Ϊ������ģʽ���������������ʵ������֮ǰ����(����open��ʱ�����ṩ��������ַ��������Զ�����)
					soc_channel.configureBlocking(false);
					// ���ӷ������������Ƿ�����ģʽ����������ᷢ���������󣬲�ֱ�ӷ���false(����ģʽ��һֱ�ȵ����ӳɹ��������Ƿ�ɹ�)
					soc_channel.connect(new InetSocketAddress(IP, PORT));
					// ע���������״̬
					soc_channel.register(selector, SelectionKey.OP_READ);
					
					if(soc_channel.finishConnect()){
						while (!thread_close_condition) {
							Set<SelectionKey> keys = selector.selectedKeys();
							for (SelectionKey key : keys) {
								// OP_READ �����ݿɶ�
								if (key.isReadable()) {
									SocketChannel ch = (SocketChannel) key.channel();
									// �õ���������������SocketChannel����register��ʱ��ĵ���������,��Ϊ����Object
									ByteBuffer buffer = (ByteBuffer) key.attachment();
									// ������ ����ͼ�дһ�£�ʵ����Ӧ�û���ѭ����ȡ����������Ϊֹ��
									ch.read(buffer);
									recvBuf.put(buffer);
									int start = 0;
									while(start<recvBuf.position()){
										if(recvBuf.get(start)==ShpPackageBase.MAGIC1 && recvBuf.get(start+3)==ShpPackageBase.MAGIC2){
											
										}
									}
									recvBuf.clear();
									// �ı������ע�¼���������λ�����|���ʱ��
									key.interestOps(SelectionKey.OP_READ|SelectionKey.OP_WRITE);
								}
							}
						}
					}else{
						System.out.println("Socket connect failed!!!");
						sleep(3000);
					}
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					break;
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}
	
	
}
