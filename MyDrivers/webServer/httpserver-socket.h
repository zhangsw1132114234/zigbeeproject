#ifndef __HTTPSERVER_SOCKET_H__
#define __HTTPSERVER_SOCKET_H__

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void http_server_socket_init(void);
void DynWebPage(int conn);
extern void http_server_socket_thread(void *p_arg);

//#define	 	WEB_DEBUG 		1
/* HTTP Method */
#define		METHOD_ERR		0		// Error Method.
#define		METHOD_GET		1		// GET Method.
#define		METHOD_HEAD		2		// HEAD Method.
#define		METHOD_POST		3		// POST Method.

/* HTTP GET Method */
#define		PTYPE_ERR		0		// Error file. 
#define		PTYPE_HTML		1		// HTML file. 
#define		PTYPE_GIF		2		// GIF file.  
#define		PTYPE_TEXT		3		// TEXT file. 
#define		PTYPE_JPEG		4		// JPEG file. 
#define		PTYPE_FLASH		5		// FLASH file.
#define		PTYPE_PNG		6		// PNG file.  
#define		PTYPE_JS		7		// JS file.   
#define 	PTYPE_CSS		8		// CSS 
#define 	PTYPE_CGI		9		// CGI 
#define 	PTYPE_ICO		10		// ico
#define 	PTYPE_APP		11		// APP
#define 	PTYPE_MP3		12		// MP3
#define 	HTTP_SERVER_PORT	80	// Http server well-known port number */


//#define WEB_DEBUG

typedef struct Web_s{
	struct  Web_s 	*next;  // 单链表节点
	const char      *cmd;   // get&post具体消息内容存放位置
	void            (*function)(void *,void *); // 不同响应的处理方法
} WEB_Server_Struct;

#define WEB_Registry_Head 	 &SENSOR_WEB_DATA_GET
//extern  WEB_Server_Struct	 POWER_M_WEB_JSON;

#define MAX_URI_SIZE	1460//(TX_RX_MAX_BUF_SIZE/2 - sizeof(char)*2)		


void make_http_response_head(unsigned char *, char, unsigned long);		/* make response header */

char* get_http_param_value(char* uri, char* param_name);	    /* get the user-specific parameter value */

/*******************************************************************************
* 函数名称:  void WEB_Service_Registration(WebDataLinkType *next)
* 函数说明： WEB数据服务注册，与应用程序间的映射建立。
* 输入参数： 相应应用程序的链表类型指针
* 返回参数： 无
*******************************************************************************/
extern void WEB_Service_Registration(WEB_Server_Struct *next);

#endif	/* end of __HTTPD_H__ */ 

