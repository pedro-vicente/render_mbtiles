# render_mbtiles
C++ MBTiles parser and rendering using the WxWidgets GUI library

# MBTiles specification
MBTiles is a specification for storing tiled map data in SQLite databases.

https://github.com/mapbox/mbtiles-spec

# UNIX build

Dependencies
------------

[wxWidgets](https://www.wxwidgets.org/)
wxWidgets is a library for creating graphical user interfaces for cross-platform applications.
<br /> 

Install dependency packages (Ubuntu):
<pre>
sudo apt-get install build-essential
sudo apt-get install autoconf
sudo apt-get install libwxgtk3.0-dev
</pre>

Get source:
<pre>
git clone https://github.com/pedro-vicente/render_mbtiles.git
</pre>

Build with:
<pre>
autoreconf -vfi
./configure
make
</pre>

# Example

countries-raster.mbtiles

![alt text](https://user-images.githubusercontent.com/6119070/56874593-e7a2ce80-6a08-11e9-9480-771acb83af55.png)

