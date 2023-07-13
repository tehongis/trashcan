

import os
import math

import pygame
from pygame.locals import *

sc_size = (800,600)
sc_flags = 0

colors=[0x2C282D,0x4E6C75,0x78A4AF,0xEDF2E8,0xBEA48E]
#0x505A51,0x5CB2A1,0xEBCD61,0xF5A53A,0xE74E3A
#0xDFE5E4,0x9DBCC9,0x85ABC1,0x487684,0x81A2AC
#0x76847B,0x424338,0x8B6F62,0xAA9C79,0xBFB69D
#0x537684,0x2E6993,0x553A33,0xA3382D,0x73AD1F

bg_color = colors[0]

def rot_center(image, angle):
    """rotate an image while keeping its center and size"""
    orig_rect = image.get_rect()
    rot_image = pygame.transform.rotate(image, angle)
    rot_rect = orig_rect.copy()
    rot_rect.center = rot_image.get_rect().center
    rot_image = rot_image.subsurface(rot_rect).copy()
    return rot_image

#def rot_center(image, rect, angle):
#    """rotate an image while keeping its center"""
#    rot_image = pygame.transform.rotate(image, angle)
#    rot_rect = rot_image.get_rect(center=rect.center)
#    return rot_image,rot_rect

class gob:

    def __init__(self,loc,angle,shape):
        self.loc = loc
        self.angle = angle
        self.shape = shape
        
    def draw(self):
        temp_shape = rot_center(self.shape,self.angle)
        x = sc_center[0] + self.loc[0] + temp_shape.get_width() / 2
        y = sc_center[1] + self.loc[1] - temp_shape.get_height() / 2
        screen.blit( temp_shape , ( x , y ) )

    def move(self,movement):
        self.loc[0] = self.loc[0] + movement[0]        
        self.loc[1] = self.loc[1] + movement[1]

    def rotate(self,angle):        
        self.angle = self.angle + math.radians(angle)
#        self.angle = self.angle + math.degrees(angle)
        if self.angle > math.pi*2:
            self.angle = self.angle - math.pi*2
        if self.angle < 0:
            self.angle = self.angle + math.pi*2

try:
    pygame.init()
    screen=pygame.display.set_mode(sc_size,sc_flags)

    ship_name = os.path.join('spaceshooter','PNG', 'playerShip1_orange.png')
    ship_sprite = pygame.image.load(ship_name).convert_alpha()
    print(ship_sprite.get_height())

    scx = int( screen.get_width() / 2 )
    scy = int( screen.get_height() / 2 )
    sc_center = ( scy,scy )

    ship = gob( [0,0] , 0 , ship_sprite )

    running = True
    while(running):
        for event in pygame.event.get():
            if event.type==pygame.QUIT:
                running=False

        keys = pygame.key.get_pressed()
        if keys[pygame.K_ESCAPE]:
            running = False

        if keys[pygame.K_LEFT]:
            ship.rotate(-1)
        elif keys[pygame.K_RIGHT]:
            ship.rotate(1)
        if keys[pygame.K_UP]:
            ship.move((0,-1))
        elif keys[pygame.K_DOWN]:
            ship.move((0,1))

        screen.fill(bg_color)
        pygame.draw.line(screen, colors[2], (scx,0),(scx,scy*2) )
        pygame.draw.line(screen, colors[2], (0,scy),(scx*2,scy) )
        ship.draw()
        pygame.display.update()        

finally:
    pygame.quit()
