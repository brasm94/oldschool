# Import a library of functions called 'pygame'
import pygame
import math
from math import pi
import numpy as np



class Point:
    def __init__(self,x,y):
        self.x = x
        self.y = y

class Point3D:
    def __init__(self,x,y,z):
        self.x = x
        self.y = y
        self.z = z
		
class Line3D():
	
    def __init__(self, start, end):
        self.start = start
        self.end = end

def loadOBJ(filename):
	
    vertices = []
    indices = []
    lines = []
	
    f = open(filename, "r")
    for line in f:
        t = str.split(line)
        if not t:
            continue
        if t[0] == "v":
            vertices.append(Point3D(float(t[1]),float(t[2]),float(t[3])))

        if t[0] == "f":
            for i in range(1,len(t) - 1):
                index1 = int(str.split(t[i],"/")[0])
                index2 = int(str.split(t[i+1],"/")[0])
                indices.append((index1,index2))

    f.close()
	
	#Add faces as lines
    for index_pair in indices:
        index1 = index_pair[0]
        index2 = index_pair[1]
        lines.append(Line3D(vertices[index1 - 1],vertices[index2 - 1]))

    #Find duplicates
    duplicates = []
    for i in range(len(lines)):
        for j in range(i+1, len(lines)):
            line1 = lines[i]
            line2 = lines[j]

            # Case 1 -> Starts match
            if line1.start.x == line2.start.x and line1.start.y == line2.start.y and line1.start.z == line2.start.z:
                if line1.end.x == line2.end.x and line1.end.y == line2.end.y and line1.end.z == line2.end.z:
                    duplicates.append(j)
            # Case 2 -> Start matches end
            if line1.start.x == line2.end.x and line1.start.y == line2.end.y and line1.start.z == line2.end.z:
                if line1.end.x == line2.start.x and line1.end.y == line2.start.y and line1.end.z == line2.start.z:
                    duplicates.append(j)

    duplicates = list(set(duplicates))
    duplicates.sort()
    duplicates = duplicates[::-1]

    #Remove duplicates
    for j in range(len(duplicates)):
        del lines[duplicates[j]]

    return lines

def loadHouse():
    house = []
    #Floor
    house.append(Line3D(Point3D(-5, 0, -5), Point3D(5, 0, -5)))
    house.append(Line3D(Point3D(5, 0, -5), Point3D(5, 0, 5)))
    house.append(Line3D(Point3D(5, 0, 5), Point3D(-5, 0, 5)))
    house.append(Line3D(Point3D(-5, 0, 5), Point3D(-5, 0, -5)))
    #Ceiling
    house.append(Line3D(Point3D(-5, 5, -5), Point3D(5, 5, -5)))
    house.append(Line3D(Point3D(5, 5, -5), Point3D(5, 5, 5)))
    house.append(Line3D(Point3D(5, 5, 5), Point3D(-5, 5, 5)))
    house.append(Line3D(Point3D(-5, 5, 5), Point3D(-5, 5, -5)))
    #Walls
    house.append(Line3D(Point3D(-5, 0, -5), Point3D(-5, 5, -5)))
    house.append(Line3D(Point3D(5, 0, -5), Point3D(5, 5, -5)))
    house.append(Line3D(Point3D(5, 0, 5), Point3D(5, 5, 5)))
    house.append(Line3D(Point3D(-5, 0, 5), Point3D(-5, 5, 5)))
    #Door
    house.append(Line3D(Point3D(-1, 0, 5), Point3D(-1, 3, 5)))
    house.append(Line3D(Point3D(-1, 3, 5), Point3D(1, 3, 5)))
    house.append(Line3D(Point3D(1, 3, 5), Point3D(1, 0, 5)))
    #Roof
    house.append(Line3D(Point3D(-5, 5, -5), Point3D(0, 8, -5)))
    house.append(Line3D(Point3D(0, 8, -5), Point3D(5, 5, -5)))
    house.append(Line3D(Point3D(-5, 5, 5), Point3D(0, 8, 5)))
    house.append(Line3D(Point3D(0, 8, 5), Point3D(5, 5, 5)))
    house.append(Line3D(Point3D(0, 8, 5), Point3D(0, 8, -5)))
    return house    

def loadCar():
    car = []
    #Front Side
    car.append(Line3D(Point3D(-3, 2, 2), Point3D(-2, 3, 2)))
    car.append(Line3D(Point3D(-2, 3, 2), Point3D(2, 3, 2)))
    car.append(Line3D(Point3D(2, 3, 2), Point3D(3, 2, 2)))
    car.append(Line3D(Point3D(3, 2, 2), Point3D(3, 1, 2)))
    car.append(Line3D(Point3D(3, 1, 2), Point3D(-3, 1, 2)))
    car.append(Line3D(Point3D(-3, 1, 2), Point3D(-3, 2, 2)))    
    #Back Side
    car.append(Line3D(Point3D(-3, 2, -2), Point3D(-2, 3, -2)))
    car.append(Line3D(Point3D(-2, 3, -2), Point3D(2, 3, -2)))
    car.append(Line3D(Point3D(2, 3, -2), Point3D(3, 2, -2)))
    car.append(Line3D(Point3D(3, 2, -2), Point3D(3, 1, -2)))
    car.append(Line3D(Point3D(3, 1, -2), Point3D(-3, 1, -2)))
    car.append(Line3D(Point3D(-3, 1, -2), Point3D(-3, 2, -2)))

    #Connectors
    car.append(Line3D(Point3D(-3, 2, 2), Point3D(-3, 2, -2)))
    car.append(Line3D(Point3D(-2, 3, 2), Point3D(-2, 3, -2)))
    car.append(Line3D(Point3D(2, 3, 2), Point3D(2, 3, -2)))
    car.append(Line3D(Point3D(3, 2, 2), Point3D(3, 2, -2)))
    car.append(Line3D(Point3D(3, 1, 2), Point3D(3, 1, -2)))
    car.append(Line3D(Point3D(-3, 1, 2), Point3D(-3, 1, -2)))   
    return car  
def loadTire():
    tire = []
    #Front Side
    tire.append(Line3D(Point3D(-1, .5, .5), Point3D(-.5, 1, .5)))
    tire.append(Line3D(Point3D(-.5, 1, .5), Point3D(.5, 1, .5)))
    tire.append(Line3D(Point3D(.5, 1, .5), Point3D(1, .5, .5)))
    tire.append(Line3D(Point3D(1, .5, .5), Point3D(1, -.5, .5)))
    tire.append(Line3D(Point3D(1, -.5, .5), Point3D(.5, -1, .5)))
    tire.append(Line3D(Point3D(.5, -1, .5), Point3D(-.5, -1, .5)))
    tire.append(Line3D(Point3D(-.5, -1, .5), Point3D(-1, -.5, .5)))
    tire.append(Line3D(Point3D(-1, -.5, .5), Point3D(-1, .5, .5)))  
    #Back Side
    tire.append(Line3D(Point3D(-1, .5, -.5), Point3D(-.5, 1, -.5)))
    tire.append(Line3D(Point3D(-.5, 1, -.5), Point3D(.5, 1, -.5)))
    tire.append(Line3D(Point3D(.5, 1, -.5), Point3D(1, .5, -.5)))
    tire.append(Line3D(Point3D(1, .5, -.5), Point3D(1, -.5, -.5)))
    tire.append(Line3D(Point3D(1, -.5, -.5), Point3D(.5, -1, -.5)))
    tire.append(Line3D(Point3D(.5, -1, -.5), Point3D(-.5, -1, -.5)))
    tire.append(Line3D(Point3D(-.5, -1, -.5), Point3D(-1, -.5, -.5)))
    tire.append(Line3D(Point3D(-1, -.5, -.5), Point3D(-1, .5, -.5)))    
    #Connectors
    tire.append(Line3D(Point3D(-1, .5, .5), Point3D(-1, .5, -.5)))
    tire.append(Line3D(Point3D(-.5, 1, .5), Point3D(-.5, 1, -.5)))
    tire.append(Line3D(Point3D(.5, 1, .5), Point3D(.5, 1, -.5)))
    tire.append(Line3D(Point3D(1, .5, .5), Point3D(1, .5, -.5)))
    tire.append(Line3D(Point3D(1, -.5, .5), Point3D(1, -.5, -.5)))
    tire.append(Line3D(Point3D(.5, -1, .5), Point3D(.5, -1, -.5)))
    tire.append(Line3D(Point3D(-.5, -1, .5), Point3D(-.5, -1, -.5)))
    tire.append(Line3D(Point3D(-1, -.5, .5), Point3D(-1, -.5, -.5)))

    return tire 
def translate(chX,chY,chZ,obj):
    for p in obj:
        p.start.x += chX
        p.start.y += chY
        p.start.z += chZ
        p.end.x += chX
        p.end.y += chY
        p.end.z += chZ
    return obj
def convertToWorld():
    global chXcar,chYcar,chZcar
    objs = []
    objs.append(loadHouse())
    objs.append(translate(chXcar,chYcar,chZcar,loadCar()))
    objs.append(translate(2,1,-1,loadTire()))
    objs.append(translate(2,1,1,loadTire()))
    objs.append(translate(-2,1,-1,loadTire()))
    objs.append(translate(-2,1,1,loadTire()))
    near = 0
    far = 100
    toWor = worldToCamera()
    camSpec = cameraCentric()
    clipMat = clipMatrix()
    toScreen = toScreenViewMatrix()
    for ob in objs:
        for p in ob:
            arrStart = np.array([[p.start.x,p.start.y,p.start.z,1]])
            arrEnd = np.array([[p.end.x,p.end.y,p.end.z,1]])
            #to world view
            arrStart = toWor * arrStart.transpose()
            arrEnd = toWor * arrEnd.transpose()
            #align camera view
            arrStart = camSpec * arrStart
            arrEnd = camSpec * arrEnd
            #clip matrix apply
            arrStart = clipMat * arrStart
            arrEnd = clipMat * arrEnd
            #normalize
            arrStart = arrStart / arrStart[3]
            arrEnd = arrEnd / arrEnd[3]
            #get rid of z-axis
            arrStart = np.delete(arrStart,2,axis = 0)
            arrEnd = np.delete(arrEnd,2,axis = 0)
            #to  screen 
            arrStart = toScreen * arrStart
            arrEnd = toScreen * arrEnd
            #set the points
            p.start.x = arrStart[0]
            p.start.y = arrStart[1]
            p.start.z = arrStart[2]
            p.end.x = arrEnd[0]
            p.end.y = arrEnd[1]
            p.end.z = arrEnd[2]
    return objs
def worldToCamera():
    global worldAngle,worldX,worldY,worldZ
    print "world x in world to camera"
    print worldX
    rad = math.radians(worldAngle)
    camSpin = np.matrix(
        [[math.cos(rad),0,-1 * math.sin(rad),0],
        [0,1,0,0],
        [math.sin(rad),0,math.cos(rad),0],
        [0,0,0,1]])
    camTrans = np.matrix(
        [[1,0,0,worldX],
        [0,1,0,worldY],
        [0,0,1,worldZ],
        [0,0,0,1]])
    return camTrans * camSpin
def cameraCentric():
    global camAngle, camX , camY ,camZ
    rad = math.radians(camAngle)
    camSpec = np.matrix(
        [[math.cos(rad),0,-math.sin(rad),0],
        [0,1,0,0],
        [math.sin(rad),0,math.cos(rad),0],
        [0,0,0,1]])
    camTrans = np.matrix(
        [[1,0,0,-camX],
        [0,1,0,-camY],
        [0,0,1,-camZ],
        [0,0,0,1]])
    return camSpec * camTrans
def clipMatrix():
    global fov , near , far
    zoom = 1 / math.tan(math.radians(fov/2))
    print "nf" + str((far + near) / (far - near))
    clipMat = np.matrix(
        [[zoom,0,0,0],
        [0,zoom,0,0],
        [0,0,(far+near)/(far - near),(-2*far * near)/(far - near)]
        ,[0,0,1,0]])
    return clipMat
def toScreenViewMatrix(width = 512,height = 512):
    toScreenMatrix = np.matrix([[width/2,0,width/2],[0,-1*height/2,height/2],[0,0,1]])
    return toScreenMatrix
# Initialize the game engine
pygame.init()
#global variables for translation
global worldAngle,worldX,worldY,chXcar,chYcar,chZcar,chAngleTire
worldAngle = 180
worldX = 0
worldY = 0
worldZ = 0
chXcar = 0
chYcar = 0
chZcar = 0
chAngleTire = 0
#clip matrix
global fov , near , far
fov = 60
near = 10.0
far = 1000.0
global camAngle , camX , camY , camZ
camAngle = 0
camX = 0
camY = 0
camZ = -40
# Define the colors we will use in RGB format
BLACK = (  0,   0,   0)
WHITE = (255, 255, 255)
BLUE =  (  0,   0, 255)
GREEN = (  0, 255,   0)
RED =   (255,   0,   0)

# Set the height and width of the screen
size = [512, 512]
screen = pygame.display.set_mode(size)

pygame.display.set_caption("Shape Drawing")

#Set needed variables
done = False
clock = pygame.time.Clock()
start = Point(0.0,0.0)
end = Point(0.0,0.0)
# linelist = loadHouse()
objs = convertToWorld()
#Loop until the user clicks the close button.
start = pygame.time.get_ticks()
while not done:
    
    # global worldAngle,worldX,worldY,worldZ
    # This limits the while loop to a max of 100 times per second.
    # Leave this out and we will use all CPU we can.
    clock.tick(100)

	# Clear the screen and set the screen background
    screen.fill(BLACK)
    # for the timer
    timePassed = (pygame.time.get_ticks() - start) / 100.0
    if(timePassed > 1):
        chXcar +=1
        start = pygame.time.get_ticks()
        objs = convertToWorld()
        chAngleTire += 1
        print "lol"

    #Controller Code#
    #####################################################################

    for event in pygame.event.get():
        if event.type == pygame.QUIT: # If user clicked close
            done=True

    pressed = pygame.key.get_pressed()

    if pressed[pygame.K_a]:
        worldX -=  math.cos(math.radians(camAngle))
        worldZ -=  -math.sin(math.radians(camAngle))
        objs = convertToWorld()
    if pressed[pygame.K_d]:
        worldX += math.cos(math.radians(camAngle))
        worldZ += -math.sin(math.radians(camAngle))
        objs = convertToWorld()
    if pressed[pygame.K_w]:
        worldX -= math.sin(math.radians(camAngle))
        worldZ -= math.cos(math.radians(camAngle))
        objs = convertToWorld()
    if pressed[pygame.K_s]:
        worldX += math.sin(math.radians(camAngle))
        worldZ += math.cos(math.radians(camAngle))
        objs = convertToWorld()
    if pressed[pygame.K_r]:
        worldY += 1
        objs = convertToWorld()
    if pressed[pygame.K_f]:
        worldY -= 1
        objs = convertToWorld()
    if pressed[pygame.K_q]:
        camAngle += 1
        objs = convertToWorld()
    if pressed[pygame.K_e]:
        camAngle -= 1
        objs = convertToWorld()
    if pressed[pygame.K_ESCAPE]:
        done = True
    # if pressed[pygame.K_a]
	#Viewer Code#
	#####################################################################
    for obj in objs:
        for s in obj:
            #BOGUS DRAWING PARAMETERS SO YOU CAN SEE THE HOUSE WHEN YOU START UP
            # pygame.draw.line(screen, BLUE, (20*s.start.x+200, -20*s.start.y+200), (20*s.end.x+200, -20*s.end.y+200))
            # print "point"
            # print s.end.x
            # print s.end.y
            pygame.draw.line(screen, WHITE,(s.start.x,s.start.y),(s.end.x,s.end.y))

    # Go ahead and update the screen with what we've drawn.
    # This MUST happen after all the other drawing commands.
    pygame.display.flip()

# Be IDLE friendly
pygame.quit()
