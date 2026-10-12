#---------------------------------------------------------------------
# Build the conclusion application with the trackball capability
# from Eck's camera API and the custom Sphere class.
#---------------------------------------------------------------------

#-----------------------------------------------
# This code chooses the libraries compatible
# with your platform.
#-----------------------------------------------

ifeq ($(shell uname),Darwin)
   # For Macintosh
   LIBS = -framework OpenGL -framework GLUT -lm
else
   # For Linux
   LIBS = -lglut -lGL -lGLU
endif

#-----------------------------------------------

a.out: solar_system.cpp camera.cpp camera.h sphere.cpp sphere.h stb_image.h
	g++ solar_system.cpp camera.cpp sphere.cpp ${LIBS}
clean:
	rm -f a.out
