#!/usr/bin/env python3

from __future__ import annotations
import os, sys, glob
from PIL import Image

PAD = 1
ImageType = Image.ImageFile.ImageFile

def ensure_parent_dirs(file_path: str):
    """Pass in a path (with or without a filename on the end) and it will create
    any of the folders in the path if they didn't already exist."""
    folder = os.path.dirname(file_path)
    if folder and not os.path.exists(folder):
        os.makedirs(folder, exist_ok=True)

class Node:
    """Node in a bin packing tree."""
    x_space: int
    y_space: int
    image: Image = None
    down: Node = None
    right: Node = None

    def __init__(self, x_space: int, y_space: int):
        self.x_space = x_space
        self.y_space = y_space
    
    def add(self, image: Image):
        if not self.image:
            if image.size[0] <= self.x_space and image.size[1] <= self.y_space:
                self.image = image
                self.right = Node(self.x_space - image.size[0] - PAD, image.size[1])
                self.down = Node(self.x_space, self.y_space - image.size[1] - PAD)
                return True
            else:
                return False
        return self.right.add(image) or self.down.add(image)

    def render(self, file, canvas: Image, offset: tuple):
        if not self.image: return
        file.write("s %s %d %d %d %d " % (os.path.basename(self.image.filename), offset[0], offset[1], self.image.size[0], self.image.size[1]))
        canvas.paste(self.image, offset)
        self.right.render(file, canvas, (offset[0] + self.image.size[0] + PAD, offset[1]))
        self.down.render(file, canvas, (offset[0], offset[1] + self.image.size[1] + PAD))




if len(sys.argv) != 4:
    raise Exception("Args should be ./packer spriteDir outFile outFolder")

spriteDir = sys.argv[1]
outFile = sys.argv[2]
outFolder = sys.argv[3]

nodes: list[Node] = []
images: list[ImageType] = []
for filename in glob.glob("%s/*.png" % spriteDir):
    image = Image.open(filename)
    if image.size[0] > 64 or image.size[1] > 32:
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
        new_node = Node(64 - PAD, 32 - PAD)
        new_node.add(image)
        nodes.append(new_node)

with open(outFile, "w") as file:
    file.write("%d\n" % len(nodes))
    i = 1
    for node in nodes:
        canvas_name = "pack_%d.png" % i
        file.write("p rom:/pics/pack_%d.sprite " % i)
        canvas = Image.new("RGBA", (64, 32))
        node.blit(file, canvas, (0, 0))
        path = "%s/%s" % (outFolder, canvas_name)
        ensure_parent_dirs(path)
        canvas.save("%s/%s" % (outFolder, canvas_name))
        i += 1
        file.write("\n")