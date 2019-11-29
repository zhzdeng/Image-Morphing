<p align="center">
<img src="./pic/result.gif" alt="drawing" height="200" width="170"/>
</p>

# **[Image-Morphing]**
This repository contains **a simple GUI** for **automating an image morph** given an input and output image.

An example input and output image has been provided under directory `input`.

## 1. **[Dependence]**
This section lists **third-party libraries** and external **system requirements** needed to build and run the code.

* ImageMagick

**Ubuntu**
```
sudo apt-get update
sudo apt-get install imagemagick
```

**MacOS**
```
brew install imagemagick
```
* Linux/ MacOS Operating System
* ~Windows (Work In Progress)~
* [Quartz](https://www.xquartz.org/) - For **MacOS** only
* Git

**Ubuntu**
```
sudo apt-get update
sudo apt-get install git
```

**MacOS**
```
brew install git
```

## 2. **[Build]**
This section provides instruction on how to setup and build the code.

1. Download the repository to your local workstation.
```
git clone git@github.com:zhzdeng/Image-Morphing.git
```
2. Jump into the file directory you cloned it to.
```
cd Image-Morphing
```
3. Compile the code.
```
make
```

## 3. **[Install]**
Copy generated binaries to either `usr/bin` or `usr/local/bin` for general use.
```
sudo make install
```

## 4. **[Usage]**
```
morphing -s     source image
         -d     destination image
         [-o]   output directory. Default = root directory.
         [-n]   the number of transitional images
```

Once the **[Build]** has been executed, run the command below.
```
./morphing -s input/1.jpg -d input/2.jpg
```

1. You should a GUI window pop up.
2. With the GUI window opened, you can annotate it like so.

Select a point on the first image and then select its corresponding poitn in the second image.
`This is entirely up to your discretion.`

<img src="./pic/example.gif" alt="drawing" height="200" width="170"/>


3. Upon finished selecting enough points, Close the window. Intermediate `.jpg` frames are generated in your **output directory**.
4. You can follow the next section to convert those frames into a `.mp4` video.

## 5. **[JPG To MP4]**
Run the command below to complete step 4 of **[Usage]**.
```
ffmpeg -r 12 -i ./%d.jpg -vcodec mpeg4 ./video.mp4
```
You should be able to get the final resulting `video.mp4` like so.

<img src="./pic/result.gif" alt="drawing" height="200" width="170"/>

## 6. **[Acknowledgments]**
- [delaunay](https://github.com/eloraiby/delaunay)
- [CImg](http://www.cimg.eu/)
- [GenericMakefile](https://github.com/mbcrawfo/GenericMakefile)
