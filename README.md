# The Stary Earth

## Introduction
As everyone knows, The Starry Night is a famous masterpiece created by [Vincent Willem van Gogh](https://en.wikipedia.org/wiki/Vincent_van_Gogh). 

In computer graphics, we are interested in creating 3D vision algorithms. It is amazing to do style migration on an earth texture picture and create my own The Starry Earth.

## Steps
1. Using style migration algorithm to create the new texture map. i.e. Migrate the style of The Starry Night to an earth texture map.
2. Using the technology in assignment 5 to map the new-generated image to a sphere object in the space.

## Details
To implement style migration, I use python with tensorflow library. 

### Prepare
1. Python 3.7 + tensorflow 2.0
2. A VGG19 convolution neneural network trained by [ImageNet](http://www.image-net.org/)
3. Content Image (The texture map)

![avatar](/style-transfer/data/content.jpg)

4. Style Image 

![avatar](/style-transfer/data/style.jpg)

### [Algorithm](https://www.programmersought.com/article/9205361438/)

#### Loss Function
There are two parts of loss function:
1. Style loss: 
Layers: 'block1_conv1', 'block2_conv1', 'block3_conv1', 'block4_conv1', 'block5_conv1'
I take one convolution layer per block from the VGG19, the reason to do this is in paper [Visualizing and Understanding Convolutional Networks](https://link.springer.com/content/pdf/10.1007/978-3-319-10590-1_53.pdf) we know that different layer of convolution network we get different feature of the image. It should be more comprehensive to get layers from different depth.

Computation: [GRAM Matrix](https://blog.csdn.net/Hi_maxin/article/details/84979580)

2. Content loss: 
Layers: 'block5_conv2'

Computation: [L2 norm](https://mathworld.wolfram.com/L2-Norm.html) between the noise image and the output image from the VGG19.

The final loss function is a linear combination of style loss function and content loss function. The parameter can be tuned.

## New-generated image
![avatar](/style-transfer/output.png)

## Texture Mapping
Apply texture mapping algorithm from https://github.com/psarahdactyl/computer-graphics-meshes

![avatar](/result.gif)
