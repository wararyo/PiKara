// first OpenVG program
// Anthony Starks (ajstarks@gmail.com)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <VG/openvg.h>
#include <VG/vgu.h>
#include "fontinfo.h"
#include "shapes.h"
#include "MTLmr3m.inc"

//const int VIRTUAL_WIDTH = 640;
const int VIRTUAL_HEIGHT = 360;

const int SONGINFO_HEIGHT = 77;

Fontinfo Font_MTLmr;
int d_width, d_height;

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
	sf.name = "ここに文字を入力";
	sf.artist = "アーティスト名";
	sf.length = 356;
	sf.id = "sm9";
	draw_songinfo(d_height - vp(48+8+SONGINFO_HEIGHT) ,sf);
	End();						   // End the picture

	fgets(s, 2, stdin);			  // look at the pic, end with [RETURN]
	finish();					   // Graphics cleanup
	exit(0);
}
