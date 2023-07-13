

import pygame
from pygame.locals import *
import math

BG_COLOR = (50,50,60)
FG_COLOR = (220,220,250)

f = open('million-pi-digits.txt','r')
pidigits = f.read()
f.close()

try:
    pygame.init()
    screen = pygame.display.set_mode((800,800))
    scx = screen.get_width()/2
    scy = screen.get_height()/2

    screen.fill(BG_COLOR)

    count=99999

    RUNNING=True
    while(RUNNING):
        for event in pygame.event.get():
            if event.type==pygame.QUIT:
                RUNNING=False

        ex = scx + (count/250) * math.sin(count/(360*4))
        ey = scy + (count/250) * math.cos(count/(360*4))
        value = int(pidigits[count])
        color = (value*(255/10),value*(255/10),value*(255/10))
        pygame.draw.line(screen,color,(scx,scy),(ex,ey))
        pygame.display.update()
        if count > 3:
            count = count - 1

finally:
    pygame.quit()
