/*
 Navicat Premium Data Transfer

 Source Server         : 192.168.61.100
 Source Server Type    : MySQL
 Source Server Version : 50742
 Source Host           : 192.168.61.100:3306
 Source Schema         : IMData

 Target Server Type    : MySQL
 Target Server Version : 50742
 File Encoding         : 65001

 Date: 14/08/2023 17:35:50
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for Admin
-- ----------------------------
DROP TABLE IF EXISTS `Admin`;
CREATE TABLE `Admin`  (
  `id` mediumint(6) UNSIGNED NOT NULL AUTO_INCREMENT COMMENT '用户id',
  `name` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '用户名',
  `pwd` char(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '密码',
  `status` enum('online','offline') CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT 'offline' COMMENT '在线状态',
  `email` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '用户邮箱',
  `phone` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '手机号码',
  `desc` varchar(500) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT 'Test' COMMENT '用户描述',
  `depart` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT 'DEFAULT' COMMENT '所属部门',
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `name`(`name`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 11 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Admin
-- ----------------------------
INSERT INTO `Admin` VALUES (1, 'test', '123456', 'offline', '2783688001@qq.com', '13926413813', 'IMCPP_TEST', 'C++');
INSERT INTO `Admin` VALUES (2, 'test_1', '123456', 'offline', '2783688001@qq.com', '13926413813', 'C++', 'C++');
INSERT INTO `Admin` VALUES (3, 'test_2', '123456', 'offline', '2783688001@qq.com', '13926413813', 'Test', 'DEFAULT');
INSERT INTO `Admin` VALUES (4, 'test_3', '123456', 'offline', '2783688001@qq.com', '13926413813', 'Test', 'DEFAULT');
INSERT INTO `Admin` VALUES (5, 'test_4', '123456', 'offline', '2783688001@qq.com', '13926413813', '123', 'DEFAULT');
INSERT INTO `Admin` VALUES (10, 'test_9', '123', 'offline', '2783688001@qq.com', '13926413813', 'Test', 'DEFAULT');

-- ----------------------------
-- Table structure for alldepart
-- ----------------------------
DROP TABLE IF EXISTS `alldepart`;
CREATE TABLE `alldepart`  (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '部门id',
  `departname` varchar(50) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL COMMENT '部门名称',
  `departdesc` varchar(200) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT 'Test' COMMENT '部门描述',
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `departname`(`departname`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 2 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of alldepart
-- ----------------------------
INSERT INTO `alldepart` VALUES (1, 'C++', 'Test');

-- ----------------------------
-- Table structure for allgroup
-- ----------------------------
DROP TABLE IF EXISTS `allgroup`;
CREATE TABLE `allgroup`  (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '组id',
  `groupname` varchar(50) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL COMMENT '组名称',
  `groupdesc` varchar(200) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT 'Test' COMMENT '组描述',
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `groupname`(`groupname`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 6 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of allgroup
-- ----------------------------
INSERT INTO `allgroup` VALUES (1, 'C++', 'default_1');
INSERT INTO `allgroup` VALUES (2, '123', '');
INSERT INTO `allgroup` VALUES (3, '1234', '');
INSERT INTO `allgroup` VALUES (4, 'GCC', '');
INSERT INTO `allgroup` VALUES (5, 'G++', '');

-- ----------------------------
-- Table structure for chatdepartmessage
-- ----------------------------
DROP TABLE IF EXISTS `chatdepartmessage`;
CREATE TABLE `chatdepartmessage`  (
  `msgid` int(11) NOT NULL AUTO_INCREMENT COMMENT '消息id',
  `departname` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `sendname` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `message` varchar(500) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`msgid`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 11 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of chatdepartmessage
-- ----------------------------
INSERT INTO `chatdepartmessage` VALUES (1, 'test_2', 'test_2', 'test_code');
INSERT INTO `chatdepartmessage` VALUES (4, '', 'test', '123');
INSERT INTO `chatdepartmessage` VALUES (5, '', 'test', '123');
INSERT INTO `chatdepartmessage` VALUES (6, '', 'test_1', 'test');
INSERT INTO `chatdepartmessage` VALUES (7, '', 'test', 'test');
INSERT INTO `chatdepartmessage` VALUES (8, '', 'test', 'test');
INSERT INTO `chatdepartmessage` VALUES (9, '', 'test_1', 'sete');
INSERT INTO `chatdepartmessage` VALUES (10, '', 'test', 'setset');

-- ----------------------------
-- Table structure for chatgroupmessage
-- ----------------------------
DROP TABLE IF EXISTS `chatgroupmessage`;
CREATE TABLE `chatgroupmessage`  (
  `msgid` int(11) NOT NULL AUTO_INCREMENT COMMENT '消息id',
  `groupname` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `sendname` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `message` varchar(500) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`msgid`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 34 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of chatgroupmessage
-- ----------------------------
INSERT INTO `chatgroupmessage` VALUES (1, 'test_2', 'test_2', 'test_code');
INSERT INTO `chatgroupmessage` VALUES (4, 'C++', 'test', '123');
INSERT INTO `chatgroupmessage` VALUES (5, 'C++', 'test_1', '123');
INSERT INTO `chatgroupmessage` VALUES (6, 'C++', 'test', '123');
INSERT INTO `chatgroupmessage` VALUES (7, 'C++', 'test_1', '123');
INSERT INTO `chatgroupmessage` VALUES (8, 'C++', 'test_1', '123');
INSERT INTO `chatgroupmessage` VALUES (9, 'C++', 'test_1', '123');
INSERT INTO `chatgroupmessage` VALUES (10, 'C++', 'test', '123');
INSERT INTO `chatgroupmessage` VALUES (11, 'C++', 'test', '123');
INSERT INTO `chatgroupmessage` VALUES (12, 'C++', 'test_1', '123');
INSERT INTO `chatgroupmessage` VALUES (13, 'C++', 'test', '123');
INSERT INTO `chatgroupmessage` VALUES (14, 'C++', 'test_1', '123');
INSERT INTO `chatgroupmessage` VALUES (15, 'C++', 'test_3', '123');
INSERT INTO `chatgroupmessage` VALUES (16, 'C++', 'test', '123');
INSERT INTO `chatgroupmessage` VALUES (17, 'C++', 'test', '123');
INSERT INTO `chatgroupmessage` VALUES (18, 'C++', 'test', '123');
INSERT INTO `chatgroupmessage` VALUES (19, 'C++', 'test_3', '123');
INSERT INTO `chatgroupmessage` VALUES (20, 'C++', 'test', '123');
INSERT INTO `chatgroupmessage` VALUES (21, 'C++', 'test', '123');
INSERT INTO `chatgroupmessage` VALUES (22, 'C++', 'test_3', '123');
INSERT INTO `chatgroupmessage` VALUES (23, 'C++', 'test', '123');
INSERT INTO `chatgroupmessage` VALUES (24, 'C++', 'test', '123');
INSERT INTO `chatgroupmessage` VALUES (25, 'C++', 'test_3', '123');
INSERT INTO `chatgroupmessage` VALUES (26, 'C++', 'test_3', '123');
INSERT INTO `chatgroupmessage` VALUES (27, 'C++', 'test', '123');
INSERT INTO `chatgroupmessage` VALUES (28, 'C++', 'test_3', '123');
INSERT INTO `chatgroupmessage` VALUES (29, 'C++', 'test_2', '123');
INSERT INTO `chatgroupmessage` VALUES (32, 'C++', 'test', '123');
INSERT INTO `chatgroupmessage` VALUES (33, 'C++', 'test', '123');

-- ----------------------------
-- Table structure for chatmessage
-- ----------------------------
DROP TABLE IF EXISTS `chatmessage`;
CREATE TABLE `chatmessage`  (
  `msgid` int(11) NOT NULL AUTO_INCREMENT COMMENT '消息id',
  `recvname` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '接收者名称',
  `sendname` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '发送者名称',
  `message` varchar(500) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '消息',
  `isread` enum('true','false') CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT 'false' COMMENT '是否为离线消息',
  PRIMARY KEY (`msgid`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 29 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of chatmessage
-- ----------------------------
INSERT INTO `chatmessage` VALUES (4, 'test_1', 'test', '123', 'false');
INSERT INTO `chatmessage` VALUES (5, 'test_1', 'test', '123', 'true');
INSERT INTO `chatmessage` VALUES (6, 'test', 'test_1', '123', 'true');
INSERT INTO `chatmessage` VALUES (7, 'test_1', 'test', '123', 'true');
INSERT INTO `chatmessage` VALUES (8, 'test', 'test_1', '123', 'true');
INSERT INTO `chatmessage` VALUES (9, 'test_1', 'test', '123', 'true');
INSERT INTO `chatmessage` VALUES (10, 'test', 'test_1', '123', 'true');
INSERT INTO `chatmessage` VALUES (11, 'test_1', 'test', '123', 'true');
INSERT INTO `chatmessage` VALUES (12, 'test_1', 'test', 'tset', 'true');
INSERT INTO `chatmessage` VALUES (13, 'test', 'test_1', 'test', 'true');
INSERT INTO `chatmessage` VALUES (14, 'test', 'test_1', '123', 'true');
INSERT INTO `chatmessage` VALUES (15, 'test_1', 'test', '123', 'true');
INSERT INTO `chatmessage` VALUES (16, 'test_1', 'test', '123', 'true');
INSERT INTO `chatmessage` VALUES (17, 'test', 'test_1', '123', 'true');
INSERT INTO `chatmessage` VALUES (18, 'test', 'test_1', '123', 'true');
INSERT INTO `chatmessage` VALUES (19, 'test_1', 'test', '123', 'true');
INSERT INTO `chatmessage` VALUES (20, 'test_1', 'test', '123', 'false');
INSERT INTO `chatmessage` VALUES (21, 'test', 'test_1', '123', 'true');
INSERT INTO `chatmessage` VALUES (22, 'test_1', 'test', '123', 'true');
INSERT INTO `chatmessage` VALUES (23, 'test', 'test_1', '123', 'false');
INSERT INTO `chatmessage` VALUES (24, 'test_1', 'test', '123', 'false');
INSERT INTO `chatmessage` VALUES (25, 'test_1', 'test', '123', 'false');
INSERT INTO `chatmessage` VALUES (26, 'test_1', 'test', '123', 'false');
INSERT INTO `chatmessage` VALUES (27, 'test', 'test_1', '123', 'true');
INSERT INTO `chatmessage` VALUES (28, 'test_1', 'test', '123', 'true');

-- ----------------------------
-- Table structure for departuser
-- ----------------------------
DROP TABLE IF EXISTS `departuser`;
CREATE TABLE `departuser`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `departname` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '部门名称',
  `username` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '用户名称',
  `userrole` enum('intendant','employee') CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL COMMENT '用户权限',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `departname`(`departname`, `username`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 5 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of departuser
-- ----------------------------
INSERT INTO `departuser` VALUES (1, 'C++', 'test_4', 'intendant');
INSERT INTO `departuser` VALUES (2, 'C++', 'test_3', 'employee');
INSERT INTO `departuser` VALUES (3, 'C++', 'test', 'employee');
INSERT INTO `departuser` VALUES (4, 'C++', 'test_1', 'employee');

-- ----------------------------
-- Table structure for friend
-- ----------------------------
DROP TABLE IF EXISTS `friend`;
CREATE TABLE `friend`  (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `username` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '用户姓名',
  `friendname` varchar(40) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '朋友姓名',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `username`(`username`, `friendname`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 4 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of friend
-- ----------------------------
INSERT INTO `friend` VALUES (2, 'test', 'test_1');
INSERT INTO `friend` VALUES (1, 'test_3', 'test_4');

-- ----------------------------
-- Table structure for groupuser
-- ----------------------------
DROP TABLE IF EXISTS `groupuser`;
CREATE TABLE `groupuser`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `groupname` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '组名称',
  `username` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '用户名称',
  `userrole` enum('creator','normal') CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT 'normal' COMMENT '用户权限',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `groupname`(`groupname`, `username`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 9 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of groupuser
-- ----------------------------
INSERT INTO `groupuser` VALUES (1, 'C++_2', 'test_4', 'creator');
INSERT INTO `groupuser` VALUES (2, 'C++', 'test_3', 'normal');
INSERT INTO `groupuser` VALUES (3, 'C++', 'test', 'normal');
INSERT INTO `groupuser` VALUES (4, 'C++', 'test_1', 'normal');
INSERT INTO `groupuser` VALUES (5, '123', 'test', 'normal');
INSERT INTO `groupuser` VALUES (6, '123456', 'test_1', 'creator');
INSERT INTO `groupuser` VALUES (7, 'GCC', 'test', 'normal');
INSERT INTO `groupuser` VALUES (8, 'G++', 'test', 'creator');

SET FOREIGN_KEY_CHECKS = 1;
