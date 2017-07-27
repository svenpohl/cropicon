// ---
// 
// Hash to cropcircle - simple SVG cropcircle-like image generator.
// 
// 
// 
// Copyright (c) 2017 Sven Pohl (sven.pohl@zen-systems.de)
// Licence: The MIT License (MIT)
// 
// Version: 1.0 - 27.July.2017
// 
// ---


#include <stdio.h>
#include <string.h>
#include <string>
#include <time.h>
#include <math.h>

using namespace std;

#define PI 3.14159265


/*

//
// Comment in this part, if you want to create Hashvalues 
//
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

//
//
// sha256 - xxx.
//
//
void sha256(char *string, char outputBuffer[65])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    int i = 0;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
} //// sha256

*/




//
// getbit - Get bit from right-to left order.
// 00000101
//
int getbit( uint8_t byte, int index )
{
int ret = 0;

if (index == 0) ret = (byte & 1) > 0; 
if (index == 1) ret = (byte & 2) > 0; 
if (index == 2) ret = (byte & 4) > 0; 
if (index == 3) ret = (byte & 8) > 0; 
if (index == 4) ret = (byte & 16) > 0; 
if (index == 5) ret = (byte & 32) > 0; 
if (index == 6) ret = (byte & 64) > 0; 
if (index == 7) ret = (byte & 128) > 0; 

return(ret);
} // getbit



//
// printbit - Print out for each bit of a byte.
//
void printbit( uint8_t byte )
{

printf("%02x - %d %d %d %d %d %d %d %d \n", byte, getbit(byte,7), 
                                                  getbit(byte,6),  
                                                  getbit(byte,5),  
                                                  getbit(byte,4),  
                                                  getbit(byte,3),  
                                                  getbit(byte,2),  
                                                  getbit(byte,1),  
                                                  getbit(byte,0)
                                                  );
} // printbit


//
//
// setbit - Set a single bit of a byte.
//
//
void setbit( uint8_t *byte, int index, bool value )
{

if (value)
{
if (index == 0) *byte = *byte | 1;
if (index == 1) *byte = *byte | 2;
if (index == 2) *byte = *byte | 4;
if (index == 3) *byte = *byte | 8;
if (index == 4) *byte = *byte | 16;
if (index == 5) *byte = *byte | 32;
if (index == 6) *byte = *byte | 64;
if (index == 7) *byte = *byte | 128;
} 
else
  {
  if (index == 0) *byte = *byte & ~1;
  if (index == 1) *byte = *byte & ~2;
  if (index == 2) *byte = *byte & ~4;
  if (index == 3) *byte = *byte & ~8;
  if (index == 4) *byte = *byte & ~16;
  if (index == 5) *byte = *byte & ~32;
  if (index == 6) *byte = *byte & ~64;
  if (index == 7) *byte = *byte & ~128;
  }

} // setbit


//
//
// hex2byte - Convert a hexvalue to an unsigned byte.
//
//
uint8_t hex2byte( char *hexbyte )
{
int retbyte;
sscanf(hexbyte,"%x", &retbyte );
return ( (uint8_t)retbyte );
} // hex2byte




//
//
// hash_get_bit - Get a single bit of a 2^256 hash value.
//
//
int hash_get_bit( char *bitbuffer, int bit)
{
int ret = -1;
int byteindex = (bit) / 8;
int bitindex = (bit) % 8;
uint8_t byte;
char hexbyte[14];
 
byteindex = 31-byteindex;
 
strncpy( hexbyte, &bitbuffer[byteindex*2], 2);
hexbyte[2] = '\0';

byte = hex2byte(hexbyte);
ret = getbit( byte, bitindex );

return(ret);
} // hash_get_bit 







//
//
// hash_set_bit - Set a single bit of a 2^256 hash value.
//
//
void hash_set_bit( char *bitbuffer, int bit, bool value)
{

int ret = -1;
int byteindex;
int bitindex;
uint8_t byte;
char hexbyte[100];

byteindex = (bit) / 8;
byteindex = 31-byteindex;
bitindex = (bit) % 8;
 
strncpy( hexbyte, &bitbuffer[byteindex*2], 2);
hexbyte[2] = '\0';

byte = hex2byte(hexbyte);

setbit(&byte, (int)bitindex, value );
sprintf(hexbyte,"%02x",byte);
strncpy( &bitbuffer[byteindex*2],hexbyte,  2);
} // hash_set_bit 



//
//
// hash_set_bits - Set multi bits of a hash buffer.
//
//
void hash_set_bits( char *buffer, int index, int cnt, uint8_t value)
{

for (int i=0; i<cnt; i++)
    {    
    int bit = getbit( value, i );
    
    if (bit == 1)
       {
       hash_set_bit( buffer, (index+i), 1);
       } else       
         if (bit == 0)
            {
            hash_set_bit( buffer, (index+i), 0);
            }     
    } // for i..
 
} // hash_set_bits




//
//
// hash_get_bits - Get multi bits of a hash buffer.
//
//
uint8_t hash_get_bits( char *buffer, int index, int cnt )
{
uint8_t ret = 0x0;

for (int i=0; i<cnt; i++)
    {
    bool bitvalue = hash_get_bit( buffer, (index+i) );    
    setbit( &ret, i, bitvalue ); 
    } // for i...

return (ret);
} // hash_get_bits




//
//
// my_grid - Draw a background-grid for development purposes.
//
//
void my_grid( char *buffer )
{
strcpy(buffer,"");

strcat(buffer,"<line x1='0' y1='0' x2='400' y2='400' style='stroke:rgb(0,255,0);stroke-width:1' /> ");
strcat(buffer,"<line x1='400' y1='0' x2='0' y2='400' style='stroke:rgb(0,255,0);stroke-width:1' /> ");


strcat(buffer,"<line x1='0' y1='200' x2='400' y2='200' style='stroke:rgb(255,0,0);stroke-width:1' /> ");
strcat(buffer,"<line x1='0' y1='100' x2='400' y2='100' style='stroke:rgb(255,0,0);stroke-width:1' /> ");
strcat(buffer,"<line x1='0' y1='300' x2='400' y2='300' style='stroke:rgb(255,0,0);stroke-width:1' /> ");

strcat(buffer,"<line x1='100' y1='0' x2='100' y2='400' style='stroke:rgb(255,0,0);stroke-width:1' /> ");
strcat(buffer,"<line x1='200' y1='0' x2='200' y2='400' style='stroke:rgb(255,0,0);stroke-width:1' /> ");
strcat(buffer,"<line x1='300' y1='0' x2='300' y2='400' style='stroke:rgb(255,0,0);stroke-width:1' /> ");

} // my_grid




//
//
// my_line - Build a text for a single line.
//
//
void my_line( char *buffer, int x1, int y1, int x2, int y2,   int linewidth, char *fillcolor )
{

if (linewidth == -1) linewidth = 10;
if (strlen(fillcolor) < 1) strcpy(fillcolor,"#aaaaaa");
sprintf(buffer,"<line x1='%d' y1='%d' x2='%d' y2='%d' style='stroke:%s;stroke-width:%d' /> ", x1,y1, x2,y2,  fillcolor,  linewidth);

} // my_line 





//
//
// my_circle - Build a text for a Circle.
//
//
void my_circle( char *buffer, char *fillcolor, int centerx, int centery, int radius, float scale, int option1)
{
char buffer2[512];
strcpy(buffer2,"");
strcpy(buffer,"");


int stroke = (int) ((int)radius * 0.125);
int radius2 = (int)( radius * scale );

sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='%d' fill='#ffffff' /> ", centerx, centery,  radius2, fillcolor,stroke );
strcat(buffer,buffer2);

 
 if (option1 == 0)
   {
   int radius2 = (int)( radius * scale * 0.625);
   sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", centerx, centery, radius2, fillcolor);
   strcat(buffer,buffer2);
   }

 if (option1 == 1)
   {
   int radius2 = (int)( radius * scale * 0.3);
   sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", centerx, centery, radius2, fillcolor);
   strcat(buffer,buffer2);
   }
  
 if (option1 == 2)
   {
   int radius2 = (int)( radius * scale * 0.4);
   sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", centerx, centery, radius2, fillcolor);
   strcat(buffer,buffer2);

   radius2 = (int)( radius * scale * 0.3);
   sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='#ffffff' /> ", centerx, centery, radius2);
   strcat(buffer,buffer2);
   }  
   
 if (option1 == 3)
   {
   sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='%d' fill='%s' /> ", centerx, centery,  radius2, fillcolor,stroke, fillcolor );
   strcat(buffer,buffer2);

   int radius2 = (int)( radius * scale * 0.3);
   sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='#ffffff' /> ", centerx, centery, radius2);
   strcat(buffer,buffer2);
   }   
 
} // my_circle
 


//
// my_rotate - Calculate a single rotation
// returns: cx,cy <- rotated coordinates.
//
void my_rotate( int *cx, int *cy, int degrees, int length)
{
*cx = 0;
*cy = 0;

*cx = sin (degrees *PI/180) * length;
*cy = cos (degrees *PI/180) * length;
} // my_rotate
 
 
 
//
// my_segment - Draw a circle segment.
// 
void my_segment( char *buffer, int centerx, int centery, int arc1, int arc2, int radius1, int radius2,  char *fillcolor)
{
char buffer2[10000];

int  cx,cy;
int  cx1,cy1;
int  cx2,cy2;

int  cx3,cy3;
int  cx4,cy4;

strcpy(buffer,"");


my_rotate( &cx , &cy , arc2, radius1);

sprintf(buffer2,"<path d='M %d,%d ", cx+centerx, cy+centery);
strcat(buffer,buffer2);

my_rotate( &cx1, &cy1, arc1, radius1);
sprintf(buffer2, " A %d %d 0 0 1  %d,%d  ", radius1,radius1,   cx1+centerx,cy1+centery );
strcat(buffer,buffer2);

my_rotate( &cx2, &cy2, arc1, radius2);
sprintf(buffer2," L %d,%d  ", cx2+centerx, cy2+centery);
strcat(buffer,buffer2);

my_rotate( &cx3, &cy3, arc2, radius2);
sprintf(buffer2, " A %d %d 1 0 0  %d,%d  ", radius2,radius2,   cx3+centerx,cy3+centery );
strcat(buffer,buffer2);

strcat(buffer," Z'");
sprintf(buffer2," fill='%s' stroke='%s' stroke-width='0'   ", fillcolor,fillcolor);
strcat(buffer,buffer2);
strcat(buffer," />");

} // my_segment

 
 
// ---
// BEGINN free included simple 'turtle' functions.
// 
unsigned int turtle_x; 
unsigned int turtle_y;
double       turtle_arc; 
 

//
// turtle_init - Set turtle start position.
// 
void turtle_init(int cx, int cy)
{
turtle_x   = cx;
turtle_y   = cy;
turtle_arc = 0;
} // turtle_init
 
 
//
// turtle_setarc - Set turtle arc.
// 
void turtle_setarc( double arc )
{
turtle_arc = arc;
} // turtle_setarc


//
// turtle_forward - Set length of turtle movement.
// 
void turtle_forward( int length )
{
int cx = sin ((float)turtle_arc *PI/180) * length;
int cy = cos ((float)turtle_arc *PI/180) * length;

turtle_x = turtle_x + cx;
turtle_y = turtle_y + cy;
} // turtle_forward


//
// turtle_forward - Rotate turtle left.
// 
void turtle_left( double value )
{
turtle_arc = turtle_arc - value;
} // turtle_left


//
// turtle_forward - Rotate turtle right.
// 
void turtle_right( double value )
{
turtle_arc = turtle_arc + value;
} // turtle_right
 
 
//
// turtle_get - Returns current turtle position.
// 
void turtle_get(int *cx, int *cy)
{
*cx = turtle_x;
*cy = turtle_y;
} // turtle_get
  
// 
// END - ...of free included minimal simple 'turtle' functions.
// --- 



//
// my_blocksegment - Create a single block segment.
//      
void my_blocksegment( char *buffer,char *fillcolor, int centerx, int centery, int arc, int radius, int height, int width)
{
char buffer2[20000];   

int start_cx,start_cy;
int cx,cy;
int cx2,cy2;
int cx3,cy3;
int cx4,cy4;

turtle_init(centerx,centery);
turtle_setarc(arc);

turtle_forward(radius);
turtle_get(&start_cx,&start_cy);
            
int tmp_arc = turtle_arc;            

turtle_right( 90 );
turtle_forward( width/2 );
turtle_get(&cx,&cy);


turtle_init(start_cx,start_cy);
turtle_arc = tmp_arc;  

turtle_right( -90 );
turtle_forward( width/2 );
turtle_get(&cx2,&cy2);






turtle_init(start_cx,start_cy);
turtle_arc = tmp_arc;  
turtle_forward(height);

turtle_right( 90 );
turtle_forward( width/2 );
turtle_get(&cx3,&cy3);



turtle_init(start_cx,start_cy);
turtle_arc = tmp_arc;  
turtle_forward(height);

turtle_right( -90 );
turtle_forward( width/2 );
turtle_get(&cx4,&cy4);


sprintf(buffer2,"<path d='M %d,%d ", cx, cy);
strcat(buffer,buffer2);


sprintf(buffer2," L %d,%d  ", cx2, cy2);
strcat(buffer,buffer2);

sprintf(buffer2," L %d,%d  ", cx4, cy4);
strcat(buffer,buffer2);

sprintf(buffer2," L %d,%d  ", cx3, cy3);
strcat(buffer,buffer2);


strcat(buffer," Z'");
sprintf(buffer2," fill='%s' stroke='%s' stroke-width='0'   ", fillcolor,fillcolor);
strcat(buffer,buffer2);
strcat(buffer," />");



} // my_blocksegment





//
// my_drop - Creates a textbuffer of a drop-like shape.
//
void my_drop( char *buffer, char *fillcolor, int cx_, int cy_, double degrees, int point)
{
int cx,cy;
int cx2,cy2;
int px,py;
char buffer2[1000];
int radius = 100;

strcpy(buffer, "");

turtle_init(200,200);
turtle_setarc(degrees);

turtle_forward(radius*1.19);
turtle_get(&cx,&cy);


sprintf(buffer2,"<path d='M %d %d ", cx,cy);   
strcat(buffer,buffer2);   

turtle_forward(40);
turtle_get(&px,&py);

sprintf(buffer2," L %d %d ",px,py);
strcat(buffer,buffer2);

turtle_forward(20);
turtle_get(&cx,&cy);

sprintf(buffer2," L %d %d ",cx,cy);
strcat(buffer,buffer2);


turtle_right( 90 );

int step_fw = 4;
int step_arc = 9;
for (int i=0; i<=13; i++)
    {
    turtle_right(step_arc);
    turtle_forward(step_fw);                    
            
    turtle_get(&cx2,&cy2);
              
    sprintf(buffer2," L %d %d ",cx2,cy2);
    strcat(buffer,buffer2);              
    } // for i..

for (int i=0; i<=11; i++)
    {
    turtle_right(-step_arc*0.9);
    turtle_forward(step_fw);                    
              
    turtle_get(&cx2,&cy2);
    sprintf(buffer2," L %d %d ",cx2,cy2);
    strcat(buffer,buffer2);
    } // for i..
 
 //
 // Finish the shape
 //
 turtle_right(150);
 turtle_forward(25);
 turtle_get(&cx2,&cy2);
 sprintf(buffer2," L %d %d ",cx2,cy2);
 strcat(buffer,buffer2);
              
 sprintf(buffer2," Z' fill='%s' />",fillcolor);          
 strcat(buffer,buffer2);
          
     
 turtle_init(200,200);
 turtle_setarc(degrees);

 turtle_forward(radius*1.19);
 turtle_get(&cx,&cy);


 sprintf(buffer2,"<path d='M %d %d ", cx,cy);   
 strcat(buffer,buffer2);   

 turtle_forward(40);
 turtle_get(&cx,&cy);

 sprintf(buffer2," L %d %d ",cx,cy);
 strcat(buffer,buffer2);

 turtle_forward(20);
 turtle_get(&cx,&cy);

 sprintf(buffer2," L %d %d ",cx,cy);
 strcat(buffer,buffer2);

 turtle_left( 90 );

 step_fw = 4;
 step_arc = 9;
 for (int i=0; i<=13; i++)
     {
     turtle_left(step_arc);
     turtle_forward(step_fw);                    
              
     turtle_get(&cx2,&cy2);
              
     sprintf(buffer2," L %d %d ",cx2,cy2);
     strcat(buffer,buffer2);
     } // for i..

for (int i=0; i<=11; i++)
    {
    turtle_left(-step_arc*0.9);
    turtle_forward(step_fw);                    
              
    turtle_get(&cx2,&cy2);
    sprintf(buffer2," L %d %d ",cx2,cy2);
    strcat(buffer,buffer2);
    } // for i..
 

 turtle_left(150);
 turtle_forward(25);
 turtle_get(&cx2,&cy2);
 sprintf(buffer2," L %d %d ",cx2,cy2);
 strcat(buffer,buffer2);
              
 sprintf(buffer2," Z' stroke='%s' stroke-width='1'   fill='%s'  />",fillcolor,fillcolor);          
 strcat(buffer,buffer2);

 sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", px , py , 20, fillcolor);
 strcat(buffer,buffer2);   

          
 if (point)
    {              
    sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", px , py , 10, "#ffffff");
    strcat(buffer,buffer2);   
    }

 
} // my_drop



 
// --- 
// 
// my_trabant1 - Central shaping function of the cropcircle. 
//
// ---
void my_trabant1( char *buffer, char *fillcolor, int option1, 
                  int trabant_type, int centerx, int centery, 
                  int radius, 
                  int trabant1, int trabant1_count, int trabant1_option,
                  int trabant2, int trabant2_count, int trabant2_option,
                  bool decorline
                  )
{
char buffer2[20255];
char buffer3[1024];     

int degree_step = 0;
int degrees = 0;



degree_step = 0;

//
// Current version: ALLWAYS trabant_type == 0.
//
if (trabant_type == 0)
{

trabant1_count++;
if (trabant1_count > 0)
    degree_step = 360 / trabant1_count; else
    degree_step = 0;

   //
   // Trabant 1
   //
   degrees = 0;
   for (int i=0; i<trabant1_count; i++)
       {
       int cx, cy;
       degrees = degrees + degree_step;
       int length = 160;
       my_rotate( &cx, &cy, degrees, length);



       if (trabant1 == 0)
          {
          if (trabant1_option == 2)
             {
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx+200, cy+200, 20, fillcolor);
             } else
               {
               float scale = 1;
               my_circle( buffer2, fillcolor, cx+200, cy+200,20, scale, trabant1_option);
               }        
            strcat(buffer,buffer2);
          } // if (trabant1 == 0)
          else
          
       if (trabant1 == 1)
          {
          int cx2, cy2;
          int length2 = 31;
          my_rotate( &cx2, &cy2, degrees, length2);
          
          my_line( buffer2, cx2+200, cy2+200, cx+200, cy+200, -1, fillcolor );
          strcat(buffer,buffer2);

          sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx+200, cy+200, 24,fillcolor);
          strcat(buffer,buffer2);

          if (trabant1_option == 1)
             {
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx+200, cy+200, 20, "#ffffff");
             } else

          if (trabant1_option == 2)
             {
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx+200, cy+200, 20, fillcolor);
             } else
               {
               float scale = 1;
               my_circle( buffer2, fillcolor, cx+200, cy+200,20, scale, trabant1_option);
               }        
          strcat(buffer,buffer2);
          } // if (trabant1 == 1)
        else
          
          
          
       if (trabant1 == 2)
          {
          int cx2, cy2;
          int length2 = 110; // Beginn outer circle
          my_rotate( &cx2, &cy2, degrees, length2);
          
          my_line( buffer2, cx2+200, cy2+200, cx+200, cy+200, -1, fillcolor  );
          strcat(buffer,buffer2);

          sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx+200, cy+200, 24,fillcolor);
          strcat(buffer,buffer2);          
                       
          int radius = 100 * 0.80;
          int degrees2 = 13.9*3;
          if (trabant1_count == 2) degrees2 = 45;
          char fillcolor2[255];
          if (option1 == 3) strcpy(fillcolor2,"#ffffff"); else
                            strcpy(fillcolor2,fillcolor); 
                                                 

          if (trabant1_count == 1)  degrees2 = 45; else
                                    degrees2 = 18;
          
             
          int cur_degrees;
            
     
          if ( trabant1_option == 0) 
             {          
             cur_degrees = degrees-44;
             my_segment( buffer2, 200,200, cur_degrees-degrees2,cur_degrees+degrees2, radius-15, radius+5, fillcolor2);	       
             strcat(buffer,buffer2);

             cur_degrees = degrees+44;
             my_segment( buffer2, 200,200, cur_degrees-degrees2,cur_degrees+degrees2, radius-15, radius+5, fillcolor2);	       
             strcat(buffer,buffer2);
             } else
             
          if ( trabant1_option == 1) 
             {          
             cur_degrees = degrees-44;
             my_segment( buffer2, 200,200, cur_degrees-degrees2,cur_degrees+degrees2, radius-15+5, radius-12+5, fillcolor2);	       
             strcat(buffer,buffer2);

             cur_degrees = degrees+44;
             my_segment( buffer2, 200,200, cur_degrees-degrees2,cur_degrees+degrees2, radius-15+5, radius-12+5, fillcolor2);	       
             strcat(buffer,buffer2);

             cur_degrees = degrees-44;
             my_segment( buffer2, 200,200, cur_degrees-degrees2,cur_degrees+degrees2, radius-5+5, radius-8+5, fillcolor2);	       
             strcat(buffer,buffer2);

             cur_degrees = degrees+44;
             my_segment( buffer2, 200,200, cur_degrees-degrees2,cur_degrees+degrees2, radius-5+5, radius-8+5, fillcolor2);	       
             strcat(buffer,buffer2);
             } else

          if ( trabant1_option == 2) 
             {                       
             my_rotate( &cx2 , &cy2 , degrees, 67);
             
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='' fill='%s' /> ", cx2+200, cy2+200, 24,fillcolor,fillcolor2);
             strcat(buffer,buffer2);

             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='' fill='%s' /> ", cx2+200, cy2+200, 15, "#ffffff","#ffffff");
             strcat(buffer,buffer2);
             } else

          if ( trabant1_option == 3) 
             {                       
             my_rotate( &cx2 , &cy2 , degrees, 67);
             
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='' fill='%s' /> ", cx2+200, cy2+200, 24,"#ffffff","#ffffff");
             strcat(buffer,buffer2);

             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='' fill='%s' /> ", cx2+200, cy2+200, 20,fillcolor,fillcolor2);
             strcat(buffer,buffer2);
             }

             
          
 

          if (trabant1_option == 1)
             {
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx+200, cy+200, 20, "#ffffff");
             } else

          if (trabant1_option == 2)
             {
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx+200, cy+200, 20, fillcolor);
             } else  
               {
               float scale = 1;
               my_circle( buffer2, fillcolor, cx+200, cy+200,20, scale, trabant1_option);
               }        
          strcat(buffer,buffer2);
          } // if (trabant1 == 2)



       if (trabant1 == 3)
          {          
          int radius = 108 * 1.1;
          my_segment( buffer2, 200,200, degrees-13.9,degrees+13.9, radius-15, radius, (char*)"#ffffff");	
       
          strcat(buffer,buffer2);
       
          int cx2, cy2;
          int length2 = 31;
          my_rotate( &cx2, &cy2, degrees, length2);
          
          my_line( buffer2, cx2+200, cy2+200, cx+200, cy+200,  -1, fillcolor  );
          strcat(buffer,buffer2);
          
       

          sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx+200, cy+200, 24,fillcolor);
          strcat(buffer,buffer2);

          if (trabant1_option == 1)
             {
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx+200, cy+200, 20, "#ffffff");
             } else
               if (trabant1_option == 2)
                  {
                  sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx+200, cy+200, 20, fillcolor);
                  } else
                    {
                    float scale = 1;
                    my_circle( buffer2, fillcolor, cx+200, cy+200,20, scale, trabant1_option);
                    }   
                    
          strcat(buffer,buffer2);
          } // if (trabant1 == 3)
          else




       if (trabant1 == 4)
          {
          int cx2, cy2;
          int length2 = 31;
          
          if (option1 == 0) length2 = 69;
          if (option1 == 2) length2 = 45;
          
          my_rotate( &cx2, &cy2, degrees, length2);

          my_line( buffer2, cx2+200, cy2+200, cx+200, cy+200 ,  16, fillcolor );
          strcat(buffer,buffer2);
          
          my_line( buffer2, cx2+200, cy2+200, cx+200, cy+200 ,  8, (char*)"#ffffff" );
          strcat(buffer,buffer2);

          sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx+200, cy+200, 24,fillcolor);
          strcat(buffer,buffer2);

          if (trabant1_option == 1)
             {
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx+200, cy+200, 20, (char*)"#ffffff");
             } else
               if (trabant1_option == 2)
                  {
                  sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx+200, cy+200, 20, fillcolor);
                  } else  
                    {
                    float scale = 1;
                    my_circle( buffer2, fillcolor, cx+200, cy+200,20, scale, trabant1_option);
                    }        
          strcat(buffer,buffer2);
          } // if (trabant1 == 4)
          else
          
          
          
       if (trabant1 == 5)
          {
          int cx2, cy2;
          int length2 = 31;
                    
          if (trabant1_option == 0)
             {
             int radius = 108 * 1.5;
             my_segment( buffer2, 200,200, degrees-13.9,degrees+13.9, radius-15, radius, fillcolor);	       
             strcat(buffer,buffer2);

             radius = 108 * 1.5 + 30;
             my_segment( buffer2, 200,200, degrees-13.9,degrees+13.9, radius-15, radius, fillcolor);	       
             strcat(buffer,buffer2);             
             } 


          if (trabant1_option == 1)
             {
             int radius = 108 * 1.5;
             my_blocksegment( buffer2, fillcolor, 200,200, degrees, radius-15, 15, 80);	       
             strcat(buffer,buffer2);

             my_blocksegment( buffer2, fillcolor, 200,200, degrees, radius+15, 15, 80);	       
             strcat(buffer,buffer2);                                           
             } // if (trabant1_option == 1)


          if (trabant1_option == 2)
             {
             int radius = 108 * 1.5;
             my_blocksegment( buffer2, fillcolor, 200,200, degrees, radius-15, 15+30, 80);	       
             strcat(buffer,buffer2);
             } // if (trabant1_option == 1)
     
     
          if (trabant1_option == 3)
             {
             int radius = 108 * 1.5;
             my_segment( buffer2, 200,200, degrees-(180/3/trabant1_count),degrees+(180/3/trabant1_count), radius-15, radius+25, fillcolor);	       
             strcat(buffer,buffer2);
             } // if (trabant1_option == 1)     
          
          } // if (trabant1 == 5)




           
       if (trabant1 == 6)
          {
          int cx, cy;
          int cx2, cy2;
          int cx3, cy3;
          
          turtle_init(200,200);
          turtle_setarc(degrees);

          turtle_forward(radius*1.5);
          turtle_get(&cx,&cy);



          turtle_init(200,200);
          turtle_setarc(degrees);
          turtle_forward(radius*1.5);          
          turtle_left(90);
          turtle_forward(radius*0.5);
          turtle_get(&cx2,&cy2);


           
          turtle_init(200,200);
          turtle_setarc(degrees);
          turtle_forward(radius*1.5);          
          turtle_right(90);
          turtle_forward(radius*0.5);
          turtle_get(&cx3,&cy3);

         
          if (trabant1_option == 1)
             {
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx, cy, 15, fillcolor);
             strcat(buffer,buffer2);   
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx2, cy2, 15, fillcolor);
             strcat(buffer,buffer2);   
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx3, cy3, 15, fillcolor);
             strcat(buffer,buffer2);   
             
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx, cy, 10, (char*)"#ffffff");
             strcat(buffer,buffer2);   
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx2, cy2, 10,  (char*)"#ffffff");
             strcat(buffer,buffer2);   
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx3, cy3, 10,  (char*)"#ffffff");
             strcat(buffer,buffer2);   
             
             } else
               if (trabant1_option == 2)
                  {
                  sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx, cy, 15, fillcolor);
                  strcat(buffer,buffer2);   
                  sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx2, cy2, 15, fillcolor);
                  strcat(buffer,buffer2);   
                  sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx3, cy3, 15, fillcolor);
                  strcat(buffer,buffer2);   
                  } else
                    {
                    float scale = 1;
                    my_circle( buffer2, fillcolor, cx, cy,15, scale, trabant1_option);
                    strcat(buffer,buffer2);   
                    my_circle( buffer2, fillcolor, cx2, cy2,15, scale, trabant1_option);
                    strcat(buffer,buffer2);   
                    my_circle( buffer2, fillcolor, cx3, cy3,15, scale, trabant1_option);
                    strcat(buffer,buffer2);   
                    }   
     
          } // if (trabant1 == 6)





         
       if (trabant1 == 7)
          {
          int cx, cy;
          int cx2, cy2;
          int cx3, cy3;
                
          
          turtle_init(200,200);
          turtle_setarc(degrees);

          turtle_forward(radius*1.5);
          turtle_get(&cx,&cy);


          turtle_init(200,200);
          turtle_setarc(degrees);
          turtle_forward(radius*1.5);          
          turtle_left(90);
          turtle_forward(radius*0.5);
          turtle_get(&cx2,&cy2);

           
          turtle_init(200,200);
          turtle_setarc(degrees);
          turtle_forward(radius*1.5);          
          turtle_right(90);
          turtle_forward(radius*0.5);
          turtle_get(&cx3,&cy3);


          float scale = 1;
           
           
          if (trabant1_option == 1 || trabant1_option == 0 || trabant1_option == 3)
             {
             int radius = 20 * 1.4;      
             my_segment( buffer2, cx,cy, degrees-(90),degrees+(90), radius-0, radius+7, fillcolor);	       
             strcat(buffer,buffer2);
             }
             
             
          sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , 15, fillcolor);
          strcat(buffer,buffer2);   
             
 
         
          if (trabant1_option == 1)
             {
             int radius = 30 * 1.4;
             my_segment( buffer2, cx,cy, degrees-(90),degrees+(90), radius-0, radius+7, fillcolor);	       
             strcat(buffer,buffer2);  
             } 

          if (trabant1_option == 2)
             {             
             int radius = 14 * 1.4;      
             my_segment( buffer2, cx,cy, degrees-(90)-0,degrees+(90)+0, radius+11, radius+18, fillcolor);	       
             strcat(buffer,buffer2);
                           
             turtle_init(200,200);
             turtle_setarc(degrees);
             turtle_forward(110);                    
             turtle_get(&cx2,&cy2);

             turtle_forward(30);                    
             turtle_get(&cx3,&cy3);
          
             int thicknes = 7;
             my_line( buffer2, cx3, cy3, cx2 , cy2 ,  thicknes, fillcolor );
             strcat(buffer,buffer2);             
             } 


          if (trabant1_option == 3)
             {             
             turtle_init(200,200);
             turtle_setarc(degrees);
             turtle_forward(110);          
          
             turtle_get(&cx2,&cy2);
          
             int thicknes = 30;
             my_line( buffer2, cx, cy, cx2 , cy2 ,  thicknes, fillcolor );
             strcat(buffer,buffer2);             
             } 
  
          
          } // if (trabant1 == 7)





       //
       // drop  
       //
       if (trabant1 == 8)
          {
          int cx, cy;
          int cx2, cy2;
          int cx3, cy3;
        
 
          trabant1_option = 3;
          
          if (trabant1_option == 0)
             {
             my_drop( buffer2, fillcolor, 0,0, (double)degrees, 0);
             strcat(buffer, buffer2 );  
             } else

          if (trabant1_option == 1)
             {
             my_drop( buffer2, fillcolor, 0,0, (double)degrees, 1);
             strcat(buffer, buffer2 );  
             } else
          
          if (trabant1_option == 2)
             {
             my_drop( buffer2, fillcolor, 0,0, (double)degrees, 1);
             strcat(buffer, buffer2 );  
             
             
             
             turtle_init(200,200);
             turtle_setarc(degrees);
             turtle_forward(radius*1.05);          
             turtle_get(&cx3,&cy3);
          
          
             int radius = 30 * 0.8;
              
             if (option1 == 3)
                {
                my_segment( buffer2, cx3,cy3, degrees-(100) +180,degrees+(100) +180, radius-0, radius+7, (char*)"#ffffff");	       
                } else
                  {
                  my_segment( buffer2, cx3,cy3, degrees-(90) +180,degrees+(90) +180, radius-0, radius+7, fillcolor);	       
                  }
             strcat(buffer,buffer2); 
             } // if (trabant1_option == 2) 
             
             
             
             
          if (trabant1_option == 3)
             {              
             my_drop( buffer2, fillcolor, 0,0, (double)degrees, 1);
             strcat(buffer, buffer2 );  
                          
             turtle_init(200,200);
             turtle_setarc(degrees);
             turtle_forward(radius*1.90);          
             turtle_get(&cx3,&cy3);
             
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx3 , cy3 , 8, fillcolor);
             strcat(buffer,buffer2); 
             } 
             
          
          } // if (trabant1 == 8)



 
 
       if (trabant1 == 9)
          {
          int cx, cy;
          int cx2, cy2;
          int cx3, cy3;
          int radius3 = 100*0.99;
          
          turtle_init(200,200);
          turtle_setarc(degrees);
          turtle_forward(radius3*1.1);          
          turtle_get(&cx,&cy);
          
        
          radius3 = 70 * 0.85;                         
          my_segment( buffer2, cx,cy, degrees-(100) ,degrees+(100) , radius3-50, radius3+0, fillcolor);	                             
          strcat(buffer,buffer2);
          

          if (trabant1_option == 1)
             {           
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , 10, (char*)"#ffffff");
             strcat(buffer,buffer2);             
             }  

          if (trabant1_option == 2)
             {             
             radius3 = 180*0.99;
          
             turtle_init(200,200);
             turtle_setarc(degrees);
             turtle_forward(radius3*1.1);          
             turtle_get(&cx,&cy);
          
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , 5, fillcolor);
             strcat(buffer,buffer2);  
             }  

          if (trabant1_option == 3)
             {             
             radius3 = 180*0.99;
          
             turtle_init(200,200);
             turtle_setarc(degrees);
             turtle_forward(radius3*1.1);          
             turtle_get(&cx,&cy);

             turtle_init(200,200);
             turtle_setarc(degrees-15);
             turtle_forward(radius3*1.1);          
             turtle_get(&cx2,&cy2);

             turtle_init(200,200);
             turtle_setarc(degrees+15);
             turtle_forward(radius3*1.1);          
             turtle_get(&cx3,&cy3);
          
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , 5, fillcolor);
             strcat(buffer,buffer2);  

             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx2 , cy2 , 5, fillcolor);
             strcat(buffer,buffer2);  

             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx3 , cy3 , 5, fillcolor);
             strcat(buffer,buffer2);  
             }  

         
      
          } // if (trabant1 == 9)





 
       if (trabant1 == 10)
          {
          int cx, cy;
          int cx2, cy2;
          int cx3, cy3;
          int radius3 = 18*0.99;
          
          turtle_init(200,200);
          turtle_setarc(degrees);
                    
          turtle_forward(radius3*1.1);          
          turtle_get(&cx,&cy);
                    
          
          if (trabant1_option == 0 || trabant1_option == 1) 
          {
          for (int i=0; i<6; i++)
              {          
              turtle_forward(30);          
              turtle_left(15);          
              turtle_get(&cx,&cy);
              

              if (trabant1_option == 0)
                 {     
                 sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , 15-i*2, fillcolor);
                 strcat(buffer,buffer2);  

                 sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , (10-i*2), (char*)"#ffffff");
                 strcat(buffer,buffer2);  
                 } 

              if (trabant1_option == 1)
                 {     
                 sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , 15-i*2, (char*)"#ffffff");
                 strcat(buffer,buffer2);  

                 sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , (10-i*2), fillcolor);
                 strcat(buffer,buffer2);  
                 } 
                                                                   

              } // for i..6
          } // if (trabant1_option == 0 || trabant1_option == 1) 


          if (trabant1_option == 2 || trabant1_option == 3) 
          {
          for (int i=0; i<3; i++)
              {          
              turtle_forward(28*2);          
              turtle_left(15*0.8);          
              turtle_get(&cx,&cy);
              

              if (trabant1_option == 2)
                 {     
                 sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , 20-i*4, fillcolor);
                 strcat(buffer,buffer2);  

                 sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , (15-i*4), "#ffffff");
                 strcat(buffer,buffer2);  
                 } 

              if (trabant1_option == 3)
                 {     
                 sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , 20-i*4, "#ffffff");
                 strcat(buffer,buffer2);  

                 sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , (15-i*4), fillcolor);
                 strcat(buffer,buffer2);  
                 }                                                                    

              } // for i..6
          } // if (trabant1_option == 0 || trabant1_option == 1) 
                   
          } // if (trabant1 == 10)



 
 
       if (trabant1 == 11)
          {
          int cx, cy;
          int cx2, cy2;
          int cx3, cy3;
          int radius3 = 10*0.99;
          
          turtle_init(200,200);
          turtle_setarc(degrees);          
          
          turtle_forward(radius3*1.1);          
          turtle_get(&cx,&cy);
                    
          
          int array_x1[100];
          int array_y1[100];          
          int array_x2[100];
          int array_y2[100];          
          int index = 0;
          int circle_x;
          int circle_y;          
 
          
          if (trabant1_option == 0)
             {
 
             for (int i2=0; i2<24; i2++)
                 {          
                                  
                 if (trabant1_count != 4)
                    {   
                    turtle_forward(22*0.5); 
                    turtle_right(14*0.5);          
                    } else
                      {   
                      turtle_forward(17*0.5); 
                      turtle_right(4*0.5);          
                      }
                 turtle_get(&cx,&cy);
              
                 double local_arc = turtle_arc;
                 int    local_x   = cx;
                 int    local_y   = cy;
              
                 turtle_left(90);          
                 turtle_forward(23-i2*2);          
                 turtle_get(&cx2,&cy2);
              
                 turtle_x   = local_x;
                 turtle_y   = local_y;
                 turtle_arc = local_arc;

                 turtle_right(90);          
                 turtle_forward(23-i2*2);          
                 turtle_get(&cx3,&cy3);
              
                 turtle_x   = local_x;
                 turtle_y   = local_y;
                 turtle_arc = local_arc;

                 array_x1[index] = cx2;
                 array_y1[index] = cy2;          

                 array_x2[index] = cx3;
                 array_y2[index] = cy3;
                 index++;
        
                 if (i2 == 23)
                    {      
                    sprintf(buffer2,"<circle cx='%d' cy='%d' r='22' stroke='' stroke-width='' fill='%s' /> ", cx , cy , fillcolor);
                    strcat(buffer,buffer2);  

                    circle_x = cx;
                    circle_y = cy;
                    }

                                                                                   
                 } // for i..24
              

 
                 sprintf(buffer2,"<path d='M %d,%d ", array_x1[0], array_y1[0]);

                 strcat(buffer,buffer2);
 
                 for (int i2=1; i2<index; i2++)
                     {
                     sprintf(buffer2," L %d,%d  ", array_x1[i2], array_y1[i2]);
                     strcat(buffer,buffer2);    
                     } // for i..

                for (int i2=index-1; i2>0; i2--)
                    {
                    sprintf(buffer2," L %d,%d  ", array_x2[i2], array_y2[i2]);
                    strcat(buffer,buffer2);    
                    } // for i..

  
             sprintf(buffer2," Z' stroke='' stroke-width='0' style='fill:%s;opacity:1;' /> \n",fillcolor);
             strcat(buffer,buffer2);    
    
    
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", 200 , 200 , 43, fillcolor);
             strcat(buffer,buffer2);  
              
             
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='18' stroke='' stroke-width='' fill='%s' /> ", circle_x , circle_y , "#ffffff");
             strcat(buffer,buffer2);  
 
              
             } // if (trabant1_option == 0) 



          if (trabant1_option == 1)
             {
 
             for (int i2=0; i2<24; i2++)
                 {                                            
                 if (trabant1_count != 4)
                    {   
                    turtle_forward(20*0.5); 
                    turtle_right(14*0.5);          
                    } else
                      {   
                      turtle_forward(15*0.5); 
                      turtle_right(4*0.5);          
                      }
                 turtle_get(&cx,&cy);
              
                 double local_arc = turtle_arc;
                 int    local_x   = cx;
                 int    local_y   = cy;
              
                 turtle_left(90);          
                 turtle_forward(23-i2 );          
                 turtle_get(&cx2,&cy2);
              
                 turtle_x   = local_x;
                 turtle_y   = local_y;
                 turtle_arc = local_arc;

                 turtle_right(90);          
                 turtle_forward(23-i2 );          
                 turtle_get(&cx3,&cy3);
              
                 turtle_x   = local_x;
                 turtle_y   = local_y;
                 turtle_arc = local_arc;

                 array_x1[index] = cx2;
                 array_y1[index] = cy2;          

                 array_x2[index] = cx3;
                 array_y2[index] = cy3;
                 index++;                                                                                                    
                 } // for i2..24
              



               
                 sprintf(buffer2,"<path d='M %d,%d ", array_x1[0], array_y1[0]);

                 strcat(buffer,buffer2);
 
                 for (int i2=1; i2<index; i2++)
                     {
                     sprintf(buffer2," L %d,%d  ", array_x1[i2], array_y1[i2]);
                     strcat(buffer,buffer2);    
                     } // for i..

                for (int i2=index-1; i2>0; i2--)
                    {
                    sprintf(buffer2," L %d,%d  ", array_x2[i2], array_y2[i2]);
                    strcat(buffer,buffer2);    
                    } // for i..

  
             sprintf(buffer2," Z' stroke='' stroke-width='0' style='fill:%s;opacity:1;' /> \n",fillcolor);
             strcat(buffer,buffer2);    
    
    
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", 200 , 200 , 35, fillcolor);
             strcat(buffer,buffer2);  
              
             } // if (trabant1_option == 1) 




          if (trabant1_option == 2)
             {
             radius3 = 90*0.99;
          
             turtle_init(200,200);
             turtle_setarc(degrees);
                    
             turtle_forward(radius3*1.1);          
             turtle_get(&cx,&cy);
          
             for (int i2=0; i2<24; i2++)
                 {          
                                  
                 if (trabant1_count != 4)
                    {   
                    turtle_forward(20*0.4); 
                    turtle_right(14*0.41);          
                    } else
                      {   
                      turtle_forward(15*0.35); 
                      turtle_right(4*0.4);          
                      }
                 turtle_get(&cx,&cy);
              
                 double local_arc = turtle_arc;
                 int    local_x   = cx;
                 int    local_y   = cy;
              
                 turtle_left(90);          
                 turtle_forward(23-i2 );          
                 turtle_get(&cx2,&cy2);
              
                 turtle_x   = local_x;
                 turtle_y   = local_y;
                 turtle_arc = local_arc;

                 turtle_right(90);          
                 turtle_forward(23-i2 );          
                 turtle_get(&cx3,&cy3);
              
                 turtle_x   = local_x;
                 turtle_y   = local_y;
                 turtle_arc = local_arc;

                 array_x1[index] = cx2;
                 array_y1[index] = cy2;          

                 array_x2[index] = cx3;
                 array_y2[index] = cy3;
                 index++;                                                                     
                 } // for i2..24
              
 
                 sprintf(buffer2,"<path d='M %d,%d ", array_x1[0], array_y1[0]);

                 strcat(buffer,buffer2);
 
                 for (int i2=1; i2<index; i2++)
                     {
                     sprintf(buffer2," L %d,%d  ", array_x1[i2], array_y1[i2]);
                     strcat(buffer,buffer2);    
                     } // for i..

                for (int i2=index-1; i2>0; i2--)
                    {
                    sprintf(buffer2," L %d,%d  ", array_x2[i2], array_y2[i2]);
                    strcat(buffer,buffer2);    
                    } // for i..

  
             sprintf(buffer2," Z' stroke='' stroke-width='0' style='fill:%s;opacity:1;' /> \n",fillcolor);
             strcat(buffer,buffer2);    
    
    
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", 200 , 200 , 35, fillcolor);
             strcat(buffer,buffer2);  
              
             } // if (trabant1_option == 2) 



 
          if (trabant1_option == 3)
             {
             radius3 = 10*0.99;
          
             turtle_init(200,200);
             turtle_setarc(degrees);
          
          
             turtle_forward(radius3*1.1);          
             turtle_get(&cx,&cy);
          
             for (int i2=0; i2<14*1; i2++)
                 {          
                 if (trabant1_count != 4)                
                    {   
                    turtle_forward(7*2); 
                    turtle_right(4);          
                    } else
                      {   
                      turtle_forward(7*2); 
                      turtle_right(1);          
                      }
                      
                 turtle_get(&cx,&cy);
              
                 double local_arc = turtle_arc;
                 int    local_x   = cx;
                 int    local_y   = cy;
              
                 turtle_left(90);          
                 turtle_forward(18-i2 );          
                 turtle_get(&cx2,&cy2);
              
                 turtle_x   = local_x;
                 turtle_y   = local_y;
                 turtle_arc = local_arc;

                 turtle_right(90);          
                 turtle_forward(18-i2 );          
                 turtle_get(&cx3,&cy3);
              
                 turtle_x   = local_x;
                 turtle_y   = local_y;
                 turtle_arc = local_arc;

                 array_x1[index] = cx2;
                 array_y1[index] = cy2;          

                 array_x2[index] = cx3;
                 array_y2[index] = cy3;
                 index++;
        
                  
                 sprintf(buffer2,"<circle cx='%d' cy='%d' r='5' stroke='' stroke-width='' fill='%s' /> ", cx2 , cy2 , fillcolor);
                 strcat(buffer,buffer2);  

                 sprintf(buffer2,"<circle cx='%d' cy='%d' r='5' stroke='' stroke-width='' fill='%s' /> ", cx3 , cy3 , fillcolor);
                 strcat(buffer,buffer2);                                                                         
                 } // for i2..24
              
    
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", 200 , 200 , 60, fillcolor);
             strcat(buffer,buffer2);                
             } // if (trabant1_option == 3) 


    
              
             } // if (trabant1 == 11)


 

 
       if (trabant1 == 12)
          {
          int cx, cy;
          int cx2, cy2;
          int cx3, cy3;
          int radius3 = 7*0.99;
      
 
         
          //
          // White background line.
          //
          turtle_init(200,200);
          turtle_setarc(degrees);
                    
          turtle_forward(radius3*1.1);          
          turtle_get(&cx,&cy);
                            
          sprintf(buffer2,"<path d='M %d,%d ", cx, cy);
          strcat(buffer,buffer2);
  
          for (int i=0; i<12; i++)
              {          
              turtle_forward(10);          
              turtle_left(12);          
              turtle_get(&cx,&cy);
              
              sprintf(buffer2," L %d,%d  ", cx, cy);
              strcat(buffer,buffer2);    
              } // for i..6


          for (int i=0; i<12; i++)
              {          
              turtle_forward(10);    
              
              if (trabant1_count != 4)        
                 turtle_right(12); else
                 turtle_left(12); 
                          
              turtle_get(&cx,&cy);
              
              sprintf(buffer2," L %d,%d  ", cx, cy);
              strcat(buffer,buffer2);    
              } // for i..6
                  
 
             if (trabant1_option == 3)
                {
                sprintf(buffer2," ' stroke='%s' stroke-width='14' fill='none' style='opacity:1;' /> \n","#ffffff");
                } else
                {
                sprintf(buffer2," ' stroke='%s' stroke-width='7' fill='none' style='opacity:1;' /> \n","#ffffff");
                } 
             strcat(buffer,buffer2);

       
          sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , 8, "#ffffff");
          strcat(buffer,buffer2); 

          sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , 5, fillcolor);
          strcat(buffer,buffer2); 
 
 
 
 
          //
          // Colored background line.
          //      
          turtle_init(200,200);
          turtle_setarc(degrees);
                    
          turtle_forward(radius3*1.1);          
          turtle_get(&cx,&cy);
                            
          sprintf(buffer2,"<path d='M %d,%d ", cx, cy);
          strcat(buffer,buffer2);
  
          for (int i=0; i<12; i++)
              {          
              turtle_forward(10);          
              turtle_left(12);          
              turtle_get(&cx,&cy);
              
              sprintf(buffer2," L %d,%d  ", cx, cy);
              strcat(buffer,buffer2);    
              } // for i..6


          for (int i=0; i<12; i++)
              {          
              turtle_forward(10);          
              if (trabant1_count != 4)        
                 turtle_right(12); else
                 turtle_left(12); 
          
              turtle_get(&cx,&cy);
              
              sprintf(buffer2," L %d,%d  ", cx, cy);
              strcat(buffer,buffer2);    
              } // for i..6

             if (trabant1_option == 3)
                {
                sprintf(buffer2," ' stroke='%s' stroke-width='8' fill='none' style='opacity:1;' /> \n",fillcolor);
                } else
                {
                sprintf(buffer2," ' stroke='%s' stroke-width='4' fill='none' style='opacity:1;' /> \n",fillcolor);
                } 
             strcat(buffer,buffer2);
                  
          //                  
          // 4 variants
          //
          if (trabant1_option == 1)
             {                      
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , 18, fillcolor);
             strcat(buffer,buffer2);  

             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , 12, "#ffffff");
             strcat(buffer,buffer2);  
             }

          if (trabant1_option == 2)
             {                      
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , 10, fillcolor);
             strcat(buffer,buffer2);  

             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx , cy , 5, "#ffffff");
             strcat(buffer,buffer2);  
             }


          sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", 200, 200 , 5, fillcolor);
          strcat(buffer,buffer2);                     
          } // if (trabant1 == 12)


 
 
       if (trabant1 == 13)
          {
          int cx, cy;
          int cx2, cy2;
          int cx3, cy3;          
          
          int thick = 20;
          int arc   = 30; 
          
          if (trabant1_count == 1) arc = 90;
          if (trabant1_count == 2) arc = 60;
          if (trabant1_count == 3) arc = 40;
           
          int  radius3 = 200 * 0.80;    
          turtle_init(200,200);
          turtle_setarc(degrees-arc);                    
          turtle_forward(radius3);          
          turtle_get(&cx,&cy);
            
          turtle_init(200,200);
          turtle_setarc(degrees+arc);                    
          turtle_forward(radius3);          
          turtle_get(&cx2,&cy2);
                                     
          my_segment( buffer2, 200,200 , degrees-(arc) ,degrees+(arc) , radius3-20, radius3+20, fillcolor);	                             
          strcat(buffer,buffer2);
                  
          sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx, cy , thick, fillcolor);
          strcat(buffer,buffer2);  

          sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx2, cy2 , thick, fillcolor);
          strcat(buffer,buffer2);  
      
      
          if (trabant1_option == 1 || trabant1_option == 2 || trabant1_option == 3)
             {             
             thick = 16;
             arc   = 30; 
             if (trabant1_count == 1) arc = 90;
             if (trabant1_count == 2) arc = 60;
             if (trabant1_count == 3) arc = 40;
             
           
             radius3 = 200 * 0.80;    
             turtle_init(200,200);
             turtle_setarc(degrees-arc);                    
             turtle_forward(radius3);          
             turtle_get(&cx,&cy);
               
             turtle_init(200,200);
             turtle_setarc(degrees+arc);                    
             turtle_forward(radius3);          
             turtle_get(&cx2,&cy2);
                                     
             my_segment( buffer2, 200,200 , degrees-(arc) ,degrees+(arc) , radius3-thick, radius3+thick, (char*)"#ffffff");	                             
             strcat(buffer,buffer2);
                  
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx, cy , thick, (char*)"#ffffff");
             strcat(buffer,buffer2);  

             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx2, cy2 , thick, (char*)"#ffffff");
             strcat(buffer,buffer2);  

             if (trabant1_option == 2 || trabant1_option == 3)
                {
                sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx, cy , (int)thick-3, fillcolor);
                strcat(buffer,buffer2);  

                sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx2, cy2 , (int)thick-3, fillcolor);
                strcat(buffer,buffer2);  
                }

             if (trabant1_option == 3)
                {
                sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx, cy , (int)thick-8, (char*)"#ffffff");
                strcat(buffer,buffer2);  

                sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx2, cy2 , (int)thick-8, "#ffffff");
                strcat(buffer,buffer2);  
                }
 

             } // if...
         
        
 
 
         
          } // if (trabant1 == 13)



       if (trabant1 == 14)
          {
          int cx, cy;
          int cx2, cy2;
          int cx3, cy3;
                    
          int thick = 3;
          int arc   = 3; 
          int root = 20; 
          int leaf = 20; 
                   
           if (trabant1_option == 1) {
                                     thick = 3;
                                     }
           if (trabant1_option == 2) {
                                     root = 45;
                                     thick = 1;
                                     }
           if (trabant1_option == 3) {
                                     leaf = 30;
                                     thick = 4;
                                     }
                  
          int  radius3 = 200 * 0.80;    
          turtle_init(200,200);
          turtle_setarc(degrees-arc);                    
          turtle_forward(radius3);          
          turtle_get(&cx,&cy);
                        
          for (int i2=-3; i2<=3; i2++)
              {  
              arc = i2*(thick*3);
              int local_degrees = degrees+arc;
                                
              my_segment( buffer2, 200,200 , local_degrees-(thick) ,local_degrees+(thick) , radius3-root, radius3+leaf, fillcolor);	                             
              strcat(buffer,buffer2);
              }
                    
          } // if (trabant1 == 14)







       if (trabant1 == 15)
          {
          int cx, cy;
          int cx2, cy2;
          int cx3, cy3;
          
          
          int thick = 2;
          int arc   = 3; 
          int root = 10; 
          int leaf = 11; 
            
            
          for (int i2=-3; i2<=3; i2++)
              {  
              arc = i2*(thick*4);
              int local_degrees = degrees+arc;
                                  
              int   radius3 = 200 * 0.80;    
              turtle_init(200,200);
              turtle_setarc(local_degrees);                    
              turtle_forward(radius3-10);          
              turtle_get(&cx,&cy);
 
              turtle_init(200,200);
              turtle_setarc(local_degrees);                    
              turtle_forward(radius3+10);          
              turtle_get(&cx2,&cy2);
             
                                
              my_segment( buffer2, 200,200 , local_degrees-(thick) ,local_degrees+(thick) , radius3-root, radius3+leaf, fillcolor);	                             
              strcat(buffer,buffer2);
              
              sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx, cy , 8, fillcolor);
              strcat(buffer,buffer2);  

              sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx2, cy2 , 8+1, fillcolor);
              strcat(buffer,buffer2);  
              } // for i2...
          
          
          } // if (trabant1 == 15)







       
       } // for i..trabant1_count
       
       
       // Center fill (no unwanted borders)
       if (trabant1 == 1 || trabant1 == 2 || trabant1 == 3 || trabant1 == 4)
          {
          int radius  = 100;
          int radius2 = (int)( radius * 1.1 * 0.3);
          if (option1 <= 2)
              sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='' fill='%s' /> ", 200, 200, radius2,fillcolor,fillcolor); else
              sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='' fill='%s' /> ", 200, 200, radius2,"#ffffff","#ffffff"); 
          strcat(buffer,buffer2);
          }   

 
  
 
   //
   // Trabant 2
   //
      
   degrees = 0;
   for (int i=0; i<trabant2_count; i++)
       {
       int cx, cy;
       degrees = degrees + degree_step;
       int length = 160*0.7;
       my_rotate( &cx, &cy, degrees, length);


       if (trabant2 == 0)
          {
          if (trabant2_option == 2)
             {
             sprintf(buffer2,"<circle cx='%d' cy='%d' r='%d' stroke='' stroke-width='' fill='%s' /> ", cx+200, cy+200, 20, fillcolor);
             } else
               {
               float scale = 1;
               my_circle( buffer2, fillcolor,  cx+200, cy+200,20, scale, trabant2_option);
               }        
            strcat(buffer,buffer2);
          } // if (trabant2 == 0)
          else                
          {
          int radius = 86 * 1.4;
          my_segment( buffer2, 200,200, degrees-(30),degrees+(30), radius-0, radius+12, fillcolor);	       
          strcat(buffer,buffer2);           
          } // if (trabant2 == 0)
                    
       } // for (int i=0; i<trabant2_count; i++)    


   
} // if (trabant_type == 0)

} // function my_trabant1
 
 

//
//
// main - 
//
// 1. argument -> SHA256 hashvalue.
// 2. argument -> output SVG-filename.
// 
// Usage:
// ./gen_cc_svg.o [hash_sha256] [output.svg]
//
// Example:
// ./gen_cc_svg.o 9636fab63995a32e7875b0b5b9c2883cd599b5370bd0da9d2ff6f4e73ea130ec mycropcircle.svg
//
//
//
int main( int argc, char *argv[] )
{
char outfile[1024];
char hashvalue[512];

char buffer [100000]; // Make it large enough!
char buffer2[100000];
char buffer3[100000];

char fillcolor[100];


//
// Define shape color for the cropcircle. Background is always transparent
//
strcpy(fillcolor,"#666666");


//
// Check if output SVG-filename is definde.
//
if ( argc >= 3 )
   {
   strcpy(outfile, argv[2]);
   } else
     {
     strcpy(outfile,"out.svg");
     }


//
// Check if hash-value is defined.
//
if ( argc >= 2 )
   {
   strcpy(hashvalue, argv[1]);
   } else
     {     
     strcpy(hashvalue,"466c08fb5a98c1fc38817bb01b527a4d6ec69250091c0f0f07d5af3416e70460");     
     
     //
     // Comment this in, if you want to create your own hash-values (i.e. openssl).
     //
     /*
     strcpy(buffer,"");
     long randnum;
     srand ( time(NULL) );
     randnum = rand() % 200000 ; //(0..99)

     sprintf(buffer,"CropCircle %ld", randnum);
     sha256( buffer, hashvalue );
     */     
     }

 

//
// Create new SVG outfile
//
FILE *fp = fopen(outfile,"wb");

 
//
// Get some bits of the hash-value (deterministic).
//
int type            = hash_get_bits( hashvalue, 0, 3 ); // in this version not relevant
int option1         = hash_get_bits( hashvalue, 3, 2 );
int trabant1        = hash_get_bits( hashvalue, 5, 4 ); 
int trabant1_count  = hash_get_bits( hashvalue, 9, 2 );
int trabant1_option = hash_get_bits( hashvalue, 11, 2 );
int trabant2        = hash_get_bits( hashvalue, 13, 3 );
int trabant2_count  = hash_get_bits( hashvalue, 16, 2 );
int trabant2_option = hash_get_bits( hashvalue, 18, 2 );
int decor1          = hash_get_bits( hashvalue, 20, 2 );
int decor2          = hash_get_bits( hashvalue, 22, 2 );
int decorline       = hash_get_bits( hashvalue, 24, 1 );
int clockbyte       = hash_get_bits( hashvalue, 35, 8 );



strcpy(buffer,"");
sprintf(buffer3,"<svg xmlns='http://www.w3.org/2000/svg'   xmlns:xlink='http://www.w3.org/1999/xlink' width='400' height='400'>");
strcat(buffer2, buffer3);

//
// Debug grid
//
if (0)
{
my_grid( buffer3 );
strcat(buffer2, buffer3);
}


//
// Clockbyte
//
int radius = 137 * 1.4;      

my_segment( buffer3, 200,200, clockbyte-(30),clockbyte+(30), 20, radius+7, (char*)"#ffffff");	       
strcat(buffer2,buffer3);

my_segment( buffer3, 200,200, clockbyte-(30),clockbyte+(30), radius-5, radius+7, fillcolor);	       
strcat(buffer2,buffer3);
my_segment( buffer3, 200,200, clockbyte-(30-1),clockbyte+(30-1), radius-3, radius+5, (char*)"#ffffff");	       
strcat(buffer2,buffer3);
   

if (type == 1)
   {
   sprintf(buffer3,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='3' fill='none' /> ", 200, 200, 195, fillcolor);
   strcat(buffer2,buffer3);
   } else
if (type == 2)
   {
   sprintf(buffer3,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='2' fill='none' /> ", 200, 200, 195+3, fillcolor);
   strcat(buffer2,buffer3);
   sprintf(buffer3,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='2' fill='none' /> ", 200, 200, 195-3, fillcolor);
   strcat(buffer2,buffer3);
   } else
if (type == 3)
   {
   sprintf(buffer3,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='1' fill='none' /> ", 200, 200, 195+3, fillcolor);
   strcat(buffer2,buffer3);
   sprintf(buffer3,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='1' fill='none' /> ", 200, 200, 195  , fillcolor);
   strcat(buffer2,buffer3);
   sprintf(buffer3,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='1' fill='none' /> ", 200, 200, 195-3, fillcolor);
   strcat(buffer2,buffer3);
   }


  
 
int centerx = 200;
int centery = 200;
radius  = 100;
float scale=1.1;
    
my_circle( buffer3, fillcolor, centerx, centery,radius, scale, option1);
strcat(buffer2,buffer3);
   
     
int trabant_type = 0;
my_trabant1( buffer3, fillcolor, option1, trabant_type, centerx, centery,radius, trabant1, trabant1_count, trabant1_option,
                                                                trabant2, trabant2_count, trabant2_option,
                                                                decorline);
strcat(buffer2,buffer3);
  
   
if (decorline == 1)
   {
   sprintf(buffer3,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='1' fill='none' /> ", 200, 200, 130, fillcolor);
   strcat(buffer2,buffer3);

   sprintf(buffer3,"<circle cx='%d' cy='%d' r='%d' stroke='%s' stroke-width='1' fill='none' /> ", 200, 200, 130+4, fillcolor);
   strcat(buffer2,buffer3);
   } // if (decorline == 1)
      
      
strcat(buffer2,"</svg>");
strcat(buffer,buffer2);


fwrite(buffer, sizeof(char),  strlen(buffer), fp);
fclose(fp);


return(0);
} // main()

// EOF