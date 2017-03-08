CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

SOURCE = src/

OBJS =		

INCLUDES =  -I/usr/local/include/opencv -I./headers  -I/usr/local/include -I/usr/include/gtk-2.0/ -I/usr/lib/i386-linux-gnu/gtk-2.0/include  -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/glib-2.0 -I/usr/lib/i386-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2  -I/usr/include/libpng12

LIBS =	-L/usr/local/lib/ -L./lib/ -lvision_lib -lopencv_objdetect -lopencv_core -lopencv_imgproc -lopencv_highgui  -lopencv_video -lopencv_legacy -lopencv_gpu -lopencv_features2d -lopencv_flann -lopencv_contrib -lopencv_calib3d -lopencv_ml -lgsl -lgslcblas
#LIBS =	-L/usr/local/lib/ -L../edu.osu.vision.common/ -lvision_lib -lgtk-x11-2.0 -lopencv_objdetect -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video -lopencv_legacy -lopencv_gpu -lopencv_features2d -lopencv_flann -lopencv_contrib -lopencv_calib3d -lopencv_ml -lgsl -lgslcblas
TARGET =	mosdetector

#$(TARGET):	$(OBJS)
$(TARGET):	
	$(CXX) -fPIC $(INCLUDES) $(SOURCE)*.cpp  $(LIBS) -o $(TARGET) $(OBJS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
