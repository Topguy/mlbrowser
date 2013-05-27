mlbrowser
=========

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

	MLBROWSER_VERSION = 1.1.0
	MLBROWSER_SITE_METHOD = local
	MLBROWSER_SITE = $(TOPDIR)/package/mlbrowser/src

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
