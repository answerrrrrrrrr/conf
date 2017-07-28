Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 0: The Setup

In this chapter you get your system setup to do C programming. The good news for anyone using Linux or Mac OSX is that you are on a system designed for programming in C. The authors of the C language were also instrumental in the creation of the Unix operating system, and both Linux and OSX are based on Unix. In fact, the install will be incredibly easy.

I have some bad news for users of Windows: learning C on Windows is painful. You can write C code for Windows, that's not a problem. The problem is all of the libraries, functions, and tools are just a little "off" from everyone else in the C world. C came from Unix and is much easier on a Unix platform. It's just a fact of life that you'll have to accept I'm afraid.

I wanted to get this bad news out right away so that you don't panic. I'm not saying to avoid Windows entirely. I am however saying that, if you want to have the easiest time learning C, then it's time to bust out some Unix and get dirty. This could also be really good for you, since knowing a little bit of Unix will also teach you some of the idioms of C programming and expand your skills.

This also means that for everyone you'll be using the command line. Yep, I said it. You've gotta get in there and type commands at the computer. Don't be afraid though because I'll be telling you what to type and what it should look like, so you'll actually be learning quite a few mind expanding skills at the same time.

# Linux

On most Linux systems you just have to install a few packages. For Debian based systems, like Ubuntu you should just have to install a few things using these commands:
    
    
    $ sudo apt-get install build-essential
    $ sudo apt-get install build-essential
    

The above is an example of a command line prompt, so to get to where you can run that, find your "Terminal" program and run it first. Then you'll get a shell prompt similar to the $ above and can type that command into it. Do not type the ``$``, just the stuff after it.

Here's how you would install the same setup on an RPM based Linux like Fedora:
    
    
    $ su -c "yum groupinstall development-tools"
    

Once you've run that, you should be able to do the first Exercise in this book and it'll work. If not then let me know.

# Mac OSX

On Mac OSX the install is even easier. First, you'll need to either download the latest XCode from Apple, or find your install DVD and install it from there. The download will be massive and could take forever, so I recommend installing from the DVD. Also, search online for "installing xcode" for instructions on how to do it.

Once you're done installing XCode, and probably restarting your computer if it didn't make you do that, you can go find your Terminal program and get it put into your Dock. You'll be using Terminal a lot in the book, so it's good to put it in a handy location.

# Windows

For Windows users I'll show you how to get a basic Ubuntu Linux system up and running in a virtual machine so that you can still do all of my exercises, but avoid all the painful Windows installation problems.

... have to figure this one out.

# Text Editor

The choice of text editor for a programmer is a tough one. For beginners I tell them to just use Gedit since it's simple and works for code. However, it doesn't work in certain internationalized situations, and chances are you already have a favorite text editor if you've been programming for a while.

With this in mind, I want you to try out a few of the standard programmer text editors for your platform and then stick with the one that you like best. If you've been using GEdit and like it then stick with it. If you want to try something different, then try it out real quick and pick one.

The most important thing is do not get stuck picking the perfect editor. Text editors all just kind of suck in odd ways. Just pick one, stick with it, and if you find something else you like try it out. Don't spend days on end configuring it and making it perfect.

Some text editors to try out are:

   Gedit on Linux and OSX.
   TextWrangler on OSX.
   Nano which runs in Terminal and works nearly everywhere.
   Emacs and Emacs for OSX. Be prepared to do some learning though.
   Vim and MacVim

There is probably a different editor for every person out there, but these are just a few of the free ones that I know work. Try a few out, and maybe some commercial ones until you find one that you like.

## WARNING: Do Not Use An IDE

An IDE, or "Integrated Development Environment" will turn you stupid. They are the worst tools if you want to be a good programmer because they hide what's going on from you, and your job is to know what's going on. They are useful if you're trying to get something done and the platform is designed around a particular IDE, but for learning to code C (and many other languages) they are pointless.

Note

If you've played guitar then you know what tablature is, but for everyone else let me explain. In music there's an established notation called the "staff notation". It's a generic, very old, and universal way to write down what someone should play on an instrument. If you play piano this notation is fairly easy to use, since it was created mostly for piano and composers.

Guitar however is a weird instrument that doesn't really work with notation, so guitarists have an alternative notation called "tablature". What tablature does is, rather than tell you the note to play, it tells you the fret and string you should play at that time. You could learn whole songs without ever knowing about a single thing you're playing. Many people do it this way, but if you want to know what you're playing, then tablature is pointless.

It may be harder than tablature, but traditional notation tells you how to play the music rather than just how to play the guitar. With traditional notation I can walk over to a piano and play the same song. I can play it on a bass. I can put it into a computer and design whole scores around it. With tablature I can just play it on a guitar.

IDEs are like tablature. Sure, you can code pretty quickly, but you can only code in that one language on that one platform. This is why companies love selling them to you. They know you're lazy, and since it only works on their platform they've got you locked in because you are lazy.

The way you break the cycle is you suck it up and finally learn to code without an IDE. A plain editor, or a programmer's editor like Vim or Emacs, makes you work with the code. It's a little harder, but the end result is you can work with any code, on any computer, in any language, and you know what's going on.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 
























