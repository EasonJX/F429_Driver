
err_t httpd_post_begin(void *connection, const char *uri, const char *
http_request,u16_t http_request_len, int content_len, char *
response_uri,u16_t response_uri_len, u8_t *post_auto_wnd)
{
#if LWIP_HTTPD_CGI
  int i = 0;
#endif
struct http_state *hs = (struct http_state *)connection;
 if(!uri || (uri[0] == '\0')) {
    return ERR_ARG;
 }
#if LWIP_HTTPD_CGI
  if (g_iNumCGIs && g_pCGIs) {
    for (i = 0; i < g_iNumCGIs; i++) {
      if (strcmp(uri, g_pCGIs[i].pcCGIName) == 0) {
	  	response_uri = g_pCGIs[i].pfnCGIHandler(i, http_cgi_paramcount, hs->params,
                                          hs->param_vals);
		response_uri_len = strlen(response_uri);
         break;
       }
    }
  }
  if(i == g_iNumCGIs) {
    return ERR_ARG; // δ�ҵ�CGI handler 
  }
#endif
  return ERR_OK;
}




#define LWIP_HTTPD_POST_MAX_PAYLOAD_LEN     512
static char http_post_payload[LWIP_HTTPD_POST_MAX_PAYLOAD_LEN];
static u16_t http_post_payload_len = 0;

err_t httpd_post_receive_data(void *connection, struct pbuf *p)
{
    struct http_state *hs = (struct http_state *)connection;
    struct pbuf *q = p;
    int count;
    u32_t http_post_payload_full_flag = 0;
    while(q != NULL)  // ������յ�������http_post_payload
    {
      if(http_post_payload_len + q->len <= LWIP_HTTPD_POST_MAX_PAYLOAD_LEN) {
          MEMCPY(http_post_payload+http_post_payload_len, q->payload, q->len);
          http_post_payload_len += q->len;
      }
      else {  // ������� �������־λ
        http_post_payload_full_flag = 1;
        break;
      }
      q = q->next;
    }
    pbuf_free(p); // �ͷ�pbuf
    if(http_post_payload_full_flag) // ������� ��������
    {
        http_post_payload_full_flag = 0;
        http_post_payload_len = 0;
    }
    else if(hs->post_content_len_left == 0) {  // POST�����Ѿ�������� ����
            count = extract_uri_parameters(hs, http_post_payload);  // ����
            http_post_payload_len = 0;
        }
    }
    return ERR_OK;
}



void httpd_post_finished(void *connection, char *response_uri, u16_t response_uri_len)
{
    struct http_state *hs = (struct http_state *)connection;
 	strncpy(response_uri, "/index.html",response_uri_len);
}
