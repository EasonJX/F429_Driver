#include "tcpecho.h"

#include "lwip/opt.h"

#if LWIP_NETCONN

#include "lwip/sys.h"
#include "lwip/api.h"

static void tcpecho_thread(void *arg)
{

  ip_addr_t serverIpAddr;
  struct netbuf *buf;
  void *data;
  u16_t len;
  err_t err;
  struct netconn *conn;

  IP4_ADDR(&serverIpAddr,192,168,1,13);// ������IP��ַ
      
  LWIP_UNUSED_ARG(arg);
  
      for( ;; )
      {
                
                /* Create a new connection identifier. */
                conn = netconn_new(NETCONN_TCP);
                
                if (conn!=NULL)
                {
        
                    /* Netconn connection to Server IP , port number 65000. */
            
                    err = netconn_connect(conn, &serverIpAddr, 65000);
                    
                    if(err == ERR_OK)
                    {

                        printf("TCP Server 192.168.1.13 :65000 ���ӳɹ�.\r\n");
                        
                        for( ;; )
                        {
                            
                            /* receive data until the other host closes the connection */
                            if((err = netconn_recv(conn, &buf)) == ERR_OK) //��������� TCP ����
                            {
                                 //��ȡһ��ָ��netbuf �ṹ�е����ݵ�ָ��

                                 if((err = netbuf_data(buf, &data, &len)) == ERR_OK)
                                 {
                                     
                                        //���յ������� ת��������1�����ﵽ͸����Ŀ��
                                     //   comSendBuf(COM1,data,len);
                                     
                                        netbuf_delete(buf);
                                     
                                 }
                                 else
                                 {
                                     printf("err:netbuf_data(buf, &data, &len):%d.\r\n",err);
                                 }
                                
                            }
                            else//if((err = netconn_recv(conn, &buf)) == ERR_OK)
                            {
                                
                                printf("err:netconn_recv(conn, &buf):%d.\r\n",err);
                                
                                netbuf_delete(buf);
                                
                                break;
                            }
                    
                        }

                    }
                
                    printf("TCP Server 192.168.1.13 :65000 ����ʧ��.\r\n");
                    
                     netconn_close(conn);
                     netconn_delete(conn);
                    
                    vTaskDelay(1000);
                }
                else//(conn!=NULL)
                {
                    printf("Can not create TCP netconn.\r\n");
                    
                    vTaskDelay(1000);
                }

            }
}


#endif
