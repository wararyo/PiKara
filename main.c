// first OpenVG program
// Anthony Starks (ajstarks@gmail.com)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <VG/openvg.h>
#include <VG/vgu.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "fontinfo.h"
#include "shapes.h"
#include "MTLmr3m.inc"
#include <time.h>

//const int VIRTUAL_WIDTH = 640;
const int VIRTUAL_HEIGHT = 360;

const int SONGINFO_HEIGHT = 77;

Fontinfo Font_MTLmr;
int d_width, d_height;

char UDP_recv_buf[2048];
char is_received = 0;


typedef struct{

	char *name;
	char *artist;
	int length;
	char *id;

} Songinfo;

//Virtual Pixel
int vp(int px){
	if(d_height == 0) return 0;
	const double vp_ratio = (double)d_height / VIRTUAL_HEIGHT;
	return px * vp_ratio;
}

void *UDPlistener(void *arg)
{
	int sock;
	struct sockaddr_in addr;

	//pthread_detach(pthread_self());

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(59263);
	addr.sin_addr.s_addr = INADDR_ANY;

	bind(sock, (struct sockaddr *)&addr, sizeof(addr));


	while(1){
		while(is_received);
		memset(UDP_recv_buf, 0, sizeof(UDP_recv_buf));

		recv(sock, UDP_recv_buf, sizeof(UDP_recv_buf), 0);

		printf("%s\n", UDP_recv_buf);

		is_received = 1;
	}

	close(sock);
}

void Initialization(){

	init(&d_width, &d_height);		   // Graphics initialization

	Font_MTLmr = loadfont(MTLmr_glyphPoints,
		MTLmr_glyphPointIndices,
		MTLmr_glyphInstructions,
		MTLmr_glyphInstructionIndices,
		MTLmr_glyphInstructionCounts,
		MTLmr_glyphAdvances,
		MTLmr_characterMap,
		MTLmr_glyphCount);

	//Network Initialization
	pid_t   p_pid;
	pthread_t	thread_id1;
	int	status;
	void 	*result;

	p_pid=getpid();

  printf("[%d]start\n",p_pid);
	pthread_attr_setdetachstate(&thread_id1, PTHREAD_CREATE_DETACHED);
	status=pthread_create(&thread_id1,NULL,UDPlistener,(void *)NULL);
	if(status!=0){
		fprintf(stderr,"pthread_create : %s",strerror(status));
	}
	else{
		printf("[%d]thread_id1=%d\n",p_pid,thread_id1);
	}

	/*status=pthread_create(&thread_id2,NULL,UDPlistener,(void *)NULL);
	if(status!=0){
		fprintf(stderr,"pthread_create : %s",strerror(status));
	}
	else{
		printf("[%d]thread_id2=%d\n",p_pid,thread_id2);
	}*/

	//pthread_join(thread_id1,&result);
	printf("[%d]thread_id1 = %d end\n",p_pid,thread_id1);
	/*pthread_join(thread_id2,&result);
	printf("[%d]thread_id2 = %d end\n",p_pid,thread_id2);*/

  printf("[%d]end\n",p_pid);
}

void draw_actionbar(){
	Fill(255,102,51,1);
	Rect(0,d_height-vp(48),d_width,vp(48));
}

void draw_songinfo(int y,Songinfo si){
	char str[8];
	sprintf(str,"%d:%d",si.length / 60,si.length % 60);

	//Fill(240,240,240,1);
	VGfloat stops[] = {0, 0.9f, 0.9f, 0.9f, 1 ,    1, 1, 1, 1, 1};
	FillLinearGradient(0,y,0,y+vp(SONGINFO_HEIGHT),stops,2);
	Roundrect(vp(8),y,d_width-vp(16),vp(SONGINFO_HEIGHT),vp(8),vp(8));
	Fill(0,0,0,1);
	Text(vp(116), y + vp(48),si.name, Font_MTLmr, vp(14));
	Fill(0,0,0,0.5f);
	Text(vp(116), y + vp(32),si.artist, Font_MTLmr, vp(10));
	TextEnd(d_width - vp(8+8), y + vp(22),str, Font_MTLmr, vp(10));
	TextEnd(d_width - vp(8+8), y + vp(8),si.id, Font_MTLmr, vp(10));

}

int main(int argc, char *argv[]) {

	Initialization();

	char s[3];

	Start(d_width, d_height);				   // Start the picture
	Background(127, 127, 127);				   // Gray background
	draw_actionbar();
	Songinfo sf;
	sf.name = "ここに文字を入力します";
	//sf.name = UDP_recv_buf;
	sf.artist = "アーティスト名";
	sf.length = 356;
	sf.id = "sm9";
	draw_songinfo(d_height - vp(48+8+SONGINFO_HEIGHT) ,sf);
	End();						   // End the picture

	char timer[64];
	char timer_str[64];
	int count = 0;

	while(1){
		//while(!is_received);
		sf.name = UDP_recv_buf;
		//ctime(&timer);
		//sprintf(&timer_str,"%s",timer);
		sprintf(&timer_str,"%d",count);
		count++;
		sf.artist = timer_str;
		Start(d_width,d_height);
		Background(127, 127, 127);				   // Gray background
		draw_actionbar();
		draw_songinfo(d_height - vp(48+8+SONGINFO_HEIGHT) , sf);
		End();
		is_received = 0;
	}

	fgets(s, 2, stdin);			  // look at the pic, end with [RETURN]
	finish();					   // Graphics cleanup
	//exit(0);
}
