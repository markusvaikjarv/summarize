# Summarize
A Linux CLI tool that can generate a summary on any topic based on Wikipedia entries<br/><br/>
### Instructions
```sudo apt install libcurl4-gnutls-dev```<br/>```git clone https://github.com/markusvaikjarv/summarize.git```<br/>```cd summarize```<br/>
To generate ~18kB binary: ```make summarize```<br/>
To generate a debug binary with undefined behaviour and address sanitizer: ```make summarize_debug```<br/>
Usage: ```./summarize TOPIC```<br/>
To run summarize from any directory, add ```export PATH=/home/username/path/to/summarize:$PATH``` to ```.bashrc```<br/>

![alt text](https://raw.githubusercontent.com/markusvaikjarv/summarize/master/screenshots/screen.png)
