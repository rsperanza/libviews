libviews
========

Qt shared library for BlackBerry with a collection of Cascades components and Qt wrapper classes for various native APIs to allow developers to create advanced graphical and multimedia applications on BlackBerry 10 by integrating Cascades and OpenGL

This repository contains the following projects:

views - the source code for the libviews library

Play - a sample app that shows how to create a VideoView to play a video 
(currently incomplete - missing the HDMI video view and additional examples of fun ways to use the library ... should get updated before the end of the hackathon)

Work - a sample app that shows work-related applications for the library
(incomplete - currently only displays a Pie chart ... will update with additionaal code demonstrating a Signature component by Saturday)

Currently implemented / work-in-progress features:

- Opening a view in C++ on the device or HDMI display
- includes a 2D graphics API renderer that can be registered with the view (some specialty views add it by default) (Graphics2D)
- input signals for multitouch, mouse, keyboard and gamepad events (multitouch returns single touch when a view is behind a ViewsControl / ForeignWindowControl (z < 0)
- keyboard and mouse support with HDMI views
- a Canvas Cascades control control for drawing 2D graphics with Javascript in QML (partially implemented at this time ... more functionality will be available over the weekend)
- a ViewsControl Cascades control that can enable and display multiple views attached as objects to it 
- partial QML implementation (adding a HDMI display for QML has not been tested)
- a VideoView with a graphics overlay that works on the device display or HDMI
- a PhotoView component for displaying any size photo (auto shrinks image to fit limited texture size in OpenGL) (will have capability to extract EXIF data and store it in a map for easy lookup in the near future)

Additional components that are not included at this time but can be made available upon request:
- VideoCapture class that supports desktop / HDMI capture as well as video capture (missing frame replacement / extraction hooks)

Roadmap - maybe coming during the hackathon (depending a lot on my free time)
- a set of basic GUI controls for inclusion in an HDMI view (a full set is planned for release in the Spring)

Since this library is still very much a work in progress and currently has zero documentation, please contact me (Roberto Speranza - rsperanza@blackberry.com) to let me know what you would like to do with your app and I can point you in the right direction of fill you in on possible caveats.  If enough people are interested in using this library and want an impromptu session on its features, I am happy to schedule something tomorrow morning since there are no other sessions at that time).

