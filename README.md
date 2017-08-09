# Image-Morphing

## Dependence
ImageMagick

Ubuntu
```
sudo apt-get install imagemagick
```

MacOS
```
brew install imagemagick
```


## Build
### Linux

```
git clone git@github.com:zhzdeng/Image-Morphing.git
cd Image-Morphing
make
./morphing -s input/1.jpg -d input/2.jpg
```
### MacOS
### Precondition
install [Quartz](https://www.xquartz.org/)
```
git clone git@github.com:zhzdeng/Image-Morphing.git
cd Image-Morphing
make
./morphing -s input/1.jpg -d input/2.jpg
```

## Install
```
sudo make install
```

## Usage
```
morphing -s     source image
         -d     destination image
         [-o]   output directory
         [-n]   the number of transitional images
```

## Example
- example

![](./pic/example.gif)

- result

![](./pic/result.gif)

## JPG To MP4
```
ffmpeg -r 12 -i ./%d.jpg -vcodec mpeg4 ./video.mp4
```

## Thanks to
- [delaunay](https://github.com/eloraiby/delaunay)
- [CImg](http://www.cimg.eu/)
- [GenericMakefile](https://github.com/mbcrawfo/GenericMakefile)

