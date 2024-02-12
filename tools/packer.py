#!/usr/bin/env python3

PAD = 1

class Node:
    def __init__(self, x_space, y_space):
        self.x_space = x_space
        self.y_space = y_space
        self.image = None
        self.down = None
        self.right = None
    
    def __str__(self):
        if self.image:
            return "(%d %d %s %s %s)" % (self.x_space, self.y_space, self.image.filename, self.down, self.right)
        return "(%d %d)" % (self.x_space, self.y_space)
    
    def add(self, image):
        if not self.image:
            if image.size[0] <= self.x_space and image.size[1] <= self.y_space:
                self.image = image
                self.right = Node(self.x_space - image.size[0] - PAD, image.size[1])
                self.down = Node(self.x_space, self.y_space - image.size[1] - PAD)
                return True
            else:
                return False
        return self.right.add(image) or self.down.add(image)

    def blit(self, canvas, offset):
        if not self.image: return
        canvas.paste(self.image, offset)
        self.right.blit(canvas, (offset[0] + self.image.size[0] + PAD, offset[1]))
        self.down.blit(canvas, (offset[0], offset[1] + self.image.size[1] + PAD))

from PIL import Image
import sys, glob

if len(sys.argv) != 4:
    raise Exception("Args should be ./packer spriteDir outFile outFolder")

spriteDir = sys.argv[1]
outFile = sys.argv[2]
outFolder = sys.argv[3]

nodes = []
images = []
for filename in glob.glob("%s/*.png" % spriteDir):
    image = Image.open(filename)
    if image.size[0] > 64 or image.size[1] > 64:
        print("Can't add image %s because it's too big" % filename)
        continue
    images.append(image)

images.sort(key = lambda img: -max(img.size[0], img.size[1]))

for image in images:
    needs_new = True
    for node in nodes:
        if node.add(image):
            needs_new = False
            break
    if needs_new:
        new_node = Node(64 - PAD, 64 - PAD)
        new_node.add(image)
        nodes.append(new_node)

i = 1
for node in nodes:
    canvas = Image.new("RGBA", (64, 64))
    node.blit(canvas, (0, 0))
    canvas.save("%s/%d.png" % (outFolder, i))
    i += 1