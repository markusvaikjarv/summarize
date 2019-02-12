# Summarize
A Linux CLI tool that can generate a summary on any topic based on Wikipedia entries
```sudo apt install libcurl4-gnutls-dev<br/>git clone https://github.com/markusvaikjarv/summarize.git<br/>cd summarize```<br/>
To generate ~18kB binary: ```make```<br/>
To generate a debug binary with undefined behaviour and address sanitizer: ```make debug```<br/>
Usage: ```./summarize TOPIC```<br/>
To run summarize from any directory, add ```export PATH=/home/markus/Dev/summarize:$PATH``` to ```.bashrc```<br/>

![alt text](https://raw.githubusercontent.com/markusvaikjarv/summarize/master/screenshots/screenshot.png)
