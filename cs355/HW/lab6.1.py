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
Z_OFF = -40

def init():
    glClearColor (0.0, 0.0, 0.0, 0.0)
    glShadeModel (GL_FLAT)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    # glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 100.0);
    gluPerspective(60,1,0,100)
    # glMatrixMode(GL_MODELVIEW)
    # glLoadIdentity()
    # glTranslated(0, 0,0)
    # global chDepth
    # chDepth = Z_OFF
    chDepth = 0

def drawWheel():
    glLineWidth(2.5)
    glColor3f(139, 118, 160)
    lineAmount = 12
    radius = 2
    twoPi = math.pi * 2.0
    glBegin(GL_LINE_LOOP)
    for i in range(0,lineAmount):
        glVertex3f(0,radius * math.cos(i * twoPi / lineAmount), radius * math.sin(i * twoPi / lineAmount))
    glEnd()
def drawHouse():
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
    global persz
    global chRot
    global totHor
    global totDepth
    global totRot
    global seconds
    #Your Code Here
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
    glTranslated(chHor,chVert,chDepth)
    glRotated(chRot,0,1,0)

    if home == True:
        init()
        chRot = -45
        chDepth = 0
        chVert = 0
        chHor = -20
        seconds = 0
    if ortho == True:
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        glOrtho(-10.0,10.0,-10.0,10.0,-100.0, 100.0)
        seconds = 0
    if pers == True:
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(60, 1, 0, 100)
    home = False
    ortho = False
    pers = False
    drawCarTurned()
    drawWheels()
    drawNeighboorhood()
    print(seconds)
    # drawWheels()
    glFlush()
    bob = 0
def drawCarTurned():
    global seconds
    print(seconds)
    glMatrixMode(GL_MODELVIEW)
    glPushMatrix()
    glTranslated(0,0,-10+seconds)
    glRotated(90,0,1,0)
    drawCar()
    glPopMatrix()
def drawWheels():
    global seconds
    wheelXOff = 2
    wheelZOff = 2
    wheelSpreed = 10
    glMatrixMode(GL_MODELVIEW)
    glPushMatrix()
    glTranslated(-wheelXOff,0,wheelZOff +seconds - 10)
    glRotated(seconds *wheelSpreed,1,0,0)
    glRotated(90,0,1,0)
    drawTire()
    glPopMatrix()    
    glPushMatrix()
    glTranslated(wheelXOff,0,wheelZOff +seconds - 10)
    glRotated( seconds * wheelSpreed ,1,0,0)
    glRotated(90,0,1,0)
    drawTire()
    glPopMatrix()  
    glPushMatrix()
    glTranslated(-wheelXOff,0,-wheelZOff +seconds -10)
    glRotated(seconds * wheelSpreed,1,0,0)
    glRotated(90,0,1,0)
    drawTire()
    glPopMatrix()    
    glPushMatrix()
    glTranslated(wheelXOff,0,-wheelZOff +seconds -10)
    glRotated(seconds * wheelSpreed,1,0,0)
    glRotated(90,0,1,0)
    drawTire()
    glPopMatrix()  
def drawCar():
    glLineWidth(2.5)
    glColor3f(0.0, 1.0, 0.0)
    glBegin(GL_LINES)
    #Front Side
    glVertex3f(-3, 2, 2)
    glVertex3f(-2, 3, 2)
    glVertex3f(-2, 3, 2)
    glVertex3f(2, 3, 2)
    glVertex3f(2, 3, 2)
    glVertex3f(3, 2, 2)
    glVertex3f(3, 2, 2)
    glVertex3f(3, 1, 2)
    glVertex3f(3, 1, 2)
    glVertex3f(-3, 1, 2)
    glVertex3f(-3, 1, 2)
    glVertex3f(-3, 2, 2)
    #Back Side
    glVertex3f(-3, 2, -2)
    glVertex3f(-2, 3, -2)
    glVertex3f(-2, 3, -2)
    glVertex3f(2, 3, -2)
    glVertex3f(2, 3, -2)
    glVertex3f(3, 2, -2)
    glVertex3f(3, 2, -2)
    glVertex3f(3, 1, -2)
    glVertex3f(3, 1, -2)
    glVertex3f(-3, 1, -2)
    glVertex3f(-3, 1, -2)
    glVertex3f(-3, 2, -2)
    #Connectors
    glVertex3f(-3, 2, 2)
    glVertex3f(-3, 2, -2)
    glVertex3f(-2, 3, 2)
    glVertex3f(-2, 3, -2)
    glVertex3f(2, 3, 2)
    glVertex3f(2, 3, -2)
    glVertex3f(3, 2, 2)
    glVertex3f(3, 2, -2)
    glVertex3f(3, 1, 2)
    glVertex3f(3, 1, -2)
    glVertex3f(-3, 1, 2)
    glVertex3f(-3, 1, -2)
    glEnd()
def drawTire():
    glLineWidth(2.5)
    glColor3f(0.0, 0.0, 1.0)
    glBegin(GL_LINES)
    #Front Side
    glVertex3f(-1, .5, .5)
    glVertex3f(-.5, 1, .5)
    glVertex3f(-.5, 1, .5)
    glVertex3f(.5, 1, .5)
    glVertex3f(.5, 1, .5)
    glVertex3f(1, .5, .5)
    glVertex3f(1, .5, .5)
    glVertex3f(1, -.5, .5)
    glVertex3f(1, -.5, .5)
    glVertex3f(.5, -1, .5)
    glVertex3f(.5, -1, .5)
    glVertex3f(-.5, -1, .5)
    glVertex3f(-.5, -1, .5)
    glVertex3f(-1, -.5, .5)
    glVertex3f(-1, -.5, .5)
    glVertex3f(-1, .5, .5)
    #Back Side
    glVertex3f(-1, .5, -.5)
    glVertex3f(-.5, 1, -.5)
    glVertex3f(-.5, 1, -.5)
    glVertex3f(.5, 1, -.5)
    glVertex3f(.5, 1, -.5)
    glVertex3f(1, .5, -.5)
    glVertex3f(1, .5, -.5)
    glVertex3f(1, -.5, -.5)
    glVertex3f(1, -.5, -.5)
    glVertex3f(.5, -1, -.5)
    glVertex3f(.5, -1, -.5)
    glVertex3f(-.5, -1, -.5)
    glVertex3f(-.5, -1, -.5)
    glVertex3f(-1, -.5, -.5)
    glVertex3f(-1, -.5, -.5)
    glVertex3f(-1, .5, -.5)
    #ConnectorsglVertex3f(-1, .5, .5)
    glVertex3f(-1, .5, -.5)
    glVertex3f(-.5, 1, .5)
    glVertex3f(-.5, 1, -.5)
    glVertex3f(.5, 1, .5)
    glVertex3f(.5, 1, -.5)
    glVertex3f(1, .5, .5)
    glVertex3f(1, .5, -.5)
    glVertex3f(1, -.5, .5)
    glVertex3f(1, -.5, -.5)
    glVertex3f(.5, -1, .5)
    glVertex3f(.5, -1, -.5)
    glVertex3f(-.5, -1, .5)
    glVertex3f(-.5, -1, -.5)
    glVertex3f(-1, -.5, .5)
    glVertex3f(-1, -.5, -.5)
    glEnd()

def drawNeighboorhood():
    horOfset = 15
    glMatrixMode(GL_MODELVIEW)
    #left part of neighboorhood
    glPushMatrix()
    glTranslated(-horOfset,0,20)
    glRotated(90,0,1,0)
    drawHouse()
    glPopMatrix()

    glPushMatrix()
    glTranslated(-horOfset,0,0)
    glRotated(90,0,1,0)
    drawHouse()
    glPopMatrix()

    glPushMatrix()
    glTranslated(-horOfset,0,-20)
    glRotated(90,0,1,0)
    drawHouse()
    glPopMatrix()

    #right part of neighboorhood

    glPushMatrix()
    glTranslated(horOfset,0,20)
    glRotated(-90,0,1,0)
    drawHouse()
    glPopMatrix()

    glPushMatrix()
    glTranslated(horOfset,0,0)
    glRotated(-90,0,1,0)
    drawHouse()
    glPopMatrix()

    glPushMatrix()
    glTranslated(horOfset,0,-20)
    glRotated(-90,0,1,0)
    drawHouse()
    glPopMatrix()

    # glPushMatrix()
    # glTranslated(0,0,0)
    # drawHouse()
    # glPopMatrix()
def updateLoc(passed):
    global seconds
    seconds += 1
    glutTimerFunc(100,updateLoc,1)
    glutPostRedisplay()
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
        chHor += -1
    if key == b'd':
        chHor += 1
    if key == b'w':
        chDepth += 1
    if key == b's':
        chDepth += -1
    if key == b'h':
        home = True
    if key == b'r':
        chVert += -1
    if key == b'f':
        chVert += 1
    if key == b'o':
        ortho = True
    if key == b'p':
        pers = True
    if key == b'q':
        chRot += 1
        # chHor = -1
    if key == b'e':
        chRot += -1
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
global seconds
seconds = 0
totHor = 0
totDepth = 0
totRot = 0
chRot = -45
chDepth = -30
ortho = False
home = False
chVert = -10
chHor = -20
pers = False
glutInit(sys.argv)
glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB)
glutInitWindowSize (int(DISPLAY_WIDTH), int(DISPLAY_HEIGHT))
glutInitWindowPosition (100, 100)
glutCreateWindow (b'OpenGL Lab')
init ()
glutDisplayFunc(display)
glutKeyboardFunc(keyboard)
glutTimerFunc(10,updateLoc,1)
glutMainLoop()
