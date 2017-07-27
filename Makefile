# ---
#                                                              
# Makefile for cropicon.o - SVG CropCircle Generator
#
# ---

DEBUG=

cropicon.o : cropicon.cpp  
	       c++  cropicon.cpp   -o cropicon.o  $(DEBUG)    

clean: 
	rm *.o	
	 
	