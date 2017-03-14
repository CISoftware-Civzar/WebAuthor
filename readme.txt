+-----------------------------------------------------+
+++++++++++++++++++++ WebAuthor +++++++++++++++++++++++
+--------------Maintained By CI Software--------------+
+-----------------------------------------------------+

About:
The intuitive, open source webpage creation environment.

Contact:
    Charles Ivan Mozar ( CI SOftware )
        --> charlesivanmozar.cisoftware@gmail.com


How to Compile:
    > Download Qt ( open source ) from http://qt.io
    > then open the WebAuthor project file

    (NOTE: for OS X / MacOS, you may need XCode
           you can download it from the Mac App Store for free)

+-------------------------------------------------------+
+----------------Source file information----------------+
+-------------------------------------------------------+

webauthor.h / webauthor.cpp: (03/07/17)
 --> This is the main window class, everything is initialized
      here

wbhtmlsyntax.h / wbhtmlsyntax.cpp (03/08/17)
 --> This class handles the HTML syntax highlighting system

wbcsssyntax.h / wbcsssyntax.cpp (03/08/17)
 --> This class handles the Cascading Style Sheet highlighting
        system

wbjssyntax.h / wbjssyntax.cpp (03/08/17)
 --> This class handles the Javascript syntax highlighting system

wbcolorlistmodel.h / wbcolorlistmodel.cpp (03/09/17)
 --> This creates the abstract model class for the color picker tool

wblibrarian.h / wblibrarian.cpp (03/11/17)
 --> This implements the "Librarian" widget ( a dictionary for html and others )

wblibrarianmodel.h / wblibrarianmodel.cpp (03/11/17)
 --> Implements the Librarian ListView model ( also the loading of .wblibrarian files )

wbcodecompletionmodel.h / wbcodecompletionmodel.cpp (03/10/17)
 --> This creates the abstract model class for code completion

wbcodeview.h / wbcodeview.cpp (03/10/17)
 --> This defines the custom QTextEdit class we use for the editors

----- On the resource.qrc file -----

gettingstarted.htm (03/07/17)
 --> Contains the "Getting Started" page that is shown when the user
     presses the F1 Key

icon.png (03/07/17)
 --> WebAuthor's icon

html_elements.wblist (03/10/17)
 --> contains the HTML autocomplete data

css_elements.wblist (03/10/17)
 --> contains the Cascading Style Sheets autocomplete data

js_elements.wblist (03/10/17)
 --> contains the javascript autocomplete data

html_doc.wblibrarian (03/11/17)
 --> the HTML Librarian document

css_doc.wblibrarian (03/11/17)
 --> The Cascading Style Sheets Librarian document

js_doc.wblibrarian (03/11/17)
 --> The Javascript Librarian document

+-------------------------------------------------------+
+--------------------------Other------------------------+
+-------------------------------------------------------+

You may also contribute in the following areas as well...
    + Documentation
    + Design
    + Support
    + Translation

----

If you are looking for a binary instead, go to http://www.facebook.com/CISoftwareWebAuthor
and download WebAuthor for your operating system.

Currently Tested:
    KDE Neon 5.9.3 Developer Edition
    Kubuntu 16.04.1 LTS

    Windows 10 Pro / Home
    Windows 7 Pro

