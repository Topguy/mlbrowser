mlbrowser for Rasbian
=====================

This fork contains fixes and adjustments so that the browser can be compiled and run on a standard Rasbian installation.
In addition to Raspbian it also needs the prebuilt Qt5 packages from http://twolife.be/raspbian ,


HOW-TO:
-------

*Start with.*

Raspbian installed from image or NOOBS.

*Add the twolife.be repository to you software sources:*

	sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-key 2578B775
	sudo bash -c "echo \"deb http://twolife.be/raspbian/ wheezy main qt gstreamer\" > /etc/apt/sources.list.d/twolife.list

(You might still get warnings about non-authenticated packages, havent looked into it.)

*Update and upgrade:*

	sudo apt-get update
	sudo apt-get upgrade

*Install minimal Qt5 requirements and make Qt5 default Qt environment.*

	sudo apt-get install libqt5webkit5-dev qtdeclarative5-dev qt5-default


*Checkout source:*

	git clone https://github.com/Topguy/mlbrowser.git
	cd mlbrowser

*Configure build*

	mkdir build
	cd build
	qmake DEFINES+=_BROWSER_ DEFINES+=_MOUSE_ DEFINES+=_PROPERTYCHANGER_ ../src/mlbrowser.pro

*build*

	make

_NOTE!!!_ It will now fail to link with a list of libraries it can't find. 
It doesnt really need those libraries, so as a hack you replace the line starting with "LIBS" in the Makefile with this line:

*LIBS*

	LIBS = $(SUBLIBS) -L/opt/vc/lib -lQt5WebKitWidgets -pthread -lrt -L/usr/lib/arm-linux-gnueabihf -lQt5Sql -lQt5Quick -lQt5OpenGL -lQt5PrintSupport -lQt5WebKit -lQt5Qml -lQt5Widgets -lQt5Network -lQt5Gui -lQt5Core -lGLESv2 -lpthread

*then rerun make*

	make


And now it should link properly.

*Test it:*

	./mlbrowser -platform eglfs http://www.raspberrypi.org


*Install it:*

	sudo make install

*View local html-file:*

	mlbrowser -platform eglfs file:///home/pi/mlbrowser/start.html

Adding this to /etc/rc.local will cause yout Pi to boot directly into the browser on each boot.
( You'll have to use /usr/local/bin/mlbroswer actually )

New program options
-------------------
 -x or -w adjusts horizontal overscan.
 
 -y or -h adjusts vertical overscan.
 
 -z adjusts zoom-factor (something like 1.3 to 1.6 looks nice on my TV)

	mlbrowser -platform eglfs -x 92 -y 48 -z 1.3 http://www.google.com


mlbrowser
=========
(original readme)

This is the respository for the mlbrowser that has been developed by Metrological. It can be used with the buildroot fork at https://github.com/albertd/buildroot-rpi or its fork https://github.com/msieben/buildroot-rpi.git.

You can add this package to your own fork of buildroot by adding the Config.in and mlbrowser.mk files.

*Config.in*

	config BR2_PACKAGE_MLBROWSER
	bool "mlbrowser"
	help
	  Metrological's simple (accellerated) browser implementation
	  http://www.metrological.com

*mlbrowser.mk*

	#############################################################
	#
	# mlbrowser
	#
	#############################################################

	MLBROWSER_VERSION = e7bba1439ff9b38dfd36c6222e4825c5d17e7677
	MLBROWSER_SITE_METHOD = git
	MLBROWSER_SITE = https://github.com/msieben/mlbrowser.git

	ifeq ($(BR2_PACKAGE_QT5WEBKIT),y)
	MLBROWSER_DEPENDENCIES = qt5webkit
	endif

	ifeq ($(BR2_PACKAGE_QT_WEBKIT),y)
	MLBROWSER_DEPENDENCIES = qt gstreamer
	endif

	define MLBROWSER_CONFIGURE_CMDS
		(cd $(@D); \
			$(TARGET_MAKE_ENV) \
			$(HOST_DIR)/usr/bin/qmake \
				DEFINES+=_BROWSER_ \
				DEFINES+=_MOUSE_ \
				DEFINES+=_PROPERTYCHANGER_ \
				./src/mlbrowser.pro \
		)
	endef

	define MLBROWSER_BUILD_CMDS
		$(TARGET_MAKE_ENV) $(MAKE) -C $(@D)
	endef

	define MLBROWSER_INSTALL_TARGET_CMDS
		$(INSTALL) -D -m 0755 $(@D)/mlbrowser $(TARGET_DIR)/usr/bin
	endef

	define MLBROWSER_UNINSTALL_TARGET_CMDS
		rm -f $(TARGET_DIR)/usr/bin/mlbrowser
	endef

	$(eval $(generic-package))

Have fun!
