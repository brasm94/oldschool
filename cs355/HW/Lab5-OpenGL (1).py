import sys

try:
    import math
    from OpenGL.GLUT import *
    from OpenGL.GL import *
    from OpenGL.GLU import *
    from OpenGL.GL import glOrtho
    from OpenGL.GLU import gluPerspective
    from OpenGL.GL import glRotated
    from OpenGL.GL import glTranslated
    from OpenGL.GL import glLoadIdentity
    from OpenGL.GL import glMatrixMode
    from OpenGL.GL import GL_MODELVIEW
    from OpenGL.GL import GL_PROJECTION
except:
    print("ERROR: PyOpenGL not installed properly. ")

DISPLAY_WIDTH = 500.0
DISPLAY_HEIGHT = 500.0
Y_OFF = -5
Z_OFF = -20

def init(): 
    glClearColor (0.0, 0.0, 0.0, 0.0)
    glShadeModel (GL_FLAT)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    # glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 100.0);
    gluPerspective(60,1,0,100)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity()
    glTranslated(0, Y_OFF, Z_OFF)
    global totDepth
    totDepth += Z_OFF

def drawHouse ():
    glLineWidth(2.5)
    glColor3f(1.0, 0.0, 0.0)
    #Floor
    glBegin(GL_LINES)
    glVertex3f(-5.0, 0.0, -5.0)
    glVertex3f(5, 0, -5)
    glVertex3f(5, 0, -5)
    glVertex3f(5, 0, 5)
    glVertex3f(5, 0, 5)
    glVertex3f(-5, 0, 5)
    glVertex3f(-5, 0, 5)
    glVertex3f(-5, 0, -5)
    #Ceiling
    glVertex3f(-5, 5, -5)
    glVertex3f(5, 5, -5)
    glVertex3f(5, 5, -5)
    glVertex3f(5, 5, 5)
    glVertex3f(5, 5, 5)
    glVertex3f(-5, 5, 5)
    glVertex3f(-5, 5, 5)
    glVertex3f(-5, 5, -5)
    #Walls
    glVertex3f(-5, 0, -5)
    glVertex3f(-5, 5, -5)
    glVertex3f(5, 0, -5)
    glVertex3f(5, 5, -5)
    glVertex3f(5, 0, 5)
    glVertex3f(5, 5, 5)
    glVertex3f(-5, 0, 5)
    glVertex3f(-5, 5, 5)
    #Door
    glVertex3f(-1, 0, 5)
    glVertex3f(-1, 3, 5)
    glVertex3f(-1, 3, 5)
    glVertex3f(1, 3, 5)
    glVertex3f(1, 3, 5)
    glVertex3f(1, 0, 5)
    #Roof
    glVertex3f(-5, 5, -5)
    glVertex3f(0, 8, -5)
    glVertex3f(0, 8, -5)
    glVertex3f(5, 5, -5)
    glVertex3f(-5, 5, 5)
    glVertex3f(0, 8, 5)
    glVertex3f(0, 8, 5)
    glVertex3f(5, 5, 5)
    glVertex3f(0, 8, 5)
    glVertex3f(0, 8, -5)
    glEnd()

def display():
    glClear (GL_COLOR_BUFFER_BIT)
    glColor3f (1.0, 1.0, 1.0)
    # viewing transformation
    global chHor
    global chVert
    global home
    global ortho
    global chDepth
    global pers
    global chRot
    global totHor
    global totDepth
    global totRot
    #Your Code Here
    glMatrixMode(GL_MODELVIEW)


    glRotated(-totRot,0,10,0)

    glTranslated(-totHor,0,-totDepth)
    glRotated(chRot, 0, 1, 0)
    glTranslated(+totHor,0,+totDepth)
    glTranslated(chHor, chVert, chDepth)
    glRotated(totRot, 0, 10, 0)
    rad = math.radians(chRot)

    if(chRot  > 0 or chRot < 0):
        totHor = totHor * math.cos(rad) + totDepth * math.sin(rad)
        totHorTemp = totHor
        totDepth = -totHorTemp * math.sin(rad) + totDepth * math.cos(rad)


    totHor += chHor
    totDepth += chDepth
    totRot += chRot

    if home == True:
        init()
        totRot = 0
        totHor = 0
    if ortho == True:
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        glOrtho(-10.0,10.0,-10.0,10.0,-100.0, 100.0)
    if pers == True:
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(60, 1, 0, 100)
    chHor = 0
    chVert = 0
    home = False
    chDepth = 0
    ortho = False
    pers = False
    chRot = 0

    drawHouse()
    glFlush()
    

def keyboard(key, x, y):
    global chHor
    global chVert
    global home
    global chDepth
    global ortho
    global pers
    global chRot
    if key == chr(27):
        import sys
        sys.exit(0)
  
    if key == b'w':
        print("W is pressed")
    if key == b'a':
        chHor = -1
    if key == b'd':
        chHor = 1
    if key == b'w':
        chDepth = 1
    if key == b's':
        chDepth = -1
    if key == b'h':
        home = True
    if key == b'r':
        chVert = -1
    if key == b'f':
        chVert = 1
    if key == b'o':
        ortho = True
    if key == b'p':
        pers = True
    if key == b'q':
        chRot = 1
        # chHor = -1
    if key == b'e':
        chRot = -1
        # chHor = 1
    #Your Code Here
  
    glutPostRedisplay()

global chHor
global chVert
global home
global ortho
global chDepth
global pers
global chRot
global totRot
global totHor
global toDepth
totHor = 0
totDepth = 0
totRot = 0
chRot = 0
chDepth = 0
ortho = False
home = False
chVert = 0
chHor = 0
pers = False
glutInit(sys.argv)
glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB)
glutInitWindowSize (int(DISPLAY_WIDTH), int(DISPLAY_HEIGHT))
glutInitWindowPosition (100, 100)
glutCreateWindow (b'OpenGL Lab')
init ()
glutDisplayFunc(display)
glutKeyboardFunc(keyboard)
glutMainLoop()
