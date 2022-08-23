include $(TOPDIR)/rules.mk

PKG_NAME:=cloud_logs
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/cloud_logs
	DEPENDS:=+libubus +libblobmsg-json +libubox +libiot-c
	CATEGORY:=Base system
	TITLE:=cloud_logs
endef

define Package/cloud_logs/description
	Deamon process which sends logs to the cloud_logs
endef

define Package/cloud_logs/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cloud_logs $(1)/usr/bin
	$(INSTALL_BIN) ./files/cloud_logs.init $(1)/etc/init.d/cloud_logs
	$(INSTALL_CONF) ./files/cloud_logs.config $(1)/etc/config/cloud_logs
endef

$(eval $(call BuildPackage,cloud_logs))
