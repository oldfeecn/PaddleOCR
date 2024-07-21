#pragma once
#ifndef DATABASESQL_H
#define DATABASESQL_H

#include <QString>

// loginInfo
#define SQL_SELECT_ALLTABLS_NAME QString("select table_name from information_schema.tables where table_schema='%1' and table_name<>'a_pcgame'")
#define SQL_SELECT_NDF_GAME_WHERE_QQ QString("select * from a_pcgame where qq='%1'")
#define SQL_SELECT_b_procedure QString("select * from b_procedure where id>%1 ORDER BY id ")
#define SQL_SELECT_ALLTABLS_MAXID QString("select * from %1 ORDER BY id desc limit 0,1")
#define SQL_SELECT_b_procedure_ID_1 QString("select * from b_procedure where id<%1 ORDER BY id desc limit 0,1")

#define SQL_INSERT_b_likeTable QString(" insert into %1  (id,`event`,remark,run_time,`data`,`left`,`right`,top,botton,imgpath,startPointAndColor,endpointAndColor,pointcolor,if_random) values(%2,'%3','%4',%5,'%6',%7,%8,%9,%10,'%11','%12','%13','%14',%15)")
#define SQL_INSERT_b_wenzishibie QString(" insert into b_wenzishibie  (id,`event`,remark,run_time,`data`,`left`,`right`,top,botton,imgpath,startPointAndColor,endpointAndColor,pointcolor,if_random) values(%1,'%2','%3',%4,'%5',%6,%7,%8,%9,'%10','%11','%12','%13',%14)")
#define SQL_SELECT_b_wenzishibie_noRemarkExist QString("SELECT * FROM b_wenzishibie where remark like '%1'")
#define SQL_SELECT_b_procedure_noRemarkExist QString("SELECT * FROM b_procedure where remark like '%1'")
#define SQL_SELECT_d_gamerole_MinRow QString("SELECT * FROM d_gamerole where id =%1 ORDER BY id  limit 0,1")

#define SQL_b_ppocr_pic_text_isExist QString("SELECT COUNT(*) AS number FROM b_ppocr_pic where remark like '%1'")
#define SQL_f_pic_InsertPath QString("INSERT into  `f_pic` (imgpath) VALUES ('%1')")
#define SQL_f_pic_s_InsertPath QString("INSERT into  `f_pic_s` (imgpath) VALUES ('%1')")
#define SQL_f_pic_countNum QString("SELECT COUNT(1) as countNum FROM `f_pic` WHERE if_random=0")
#define SQL_f_pic_limit QString("SELECT * FROM `f_pic` WHERE if_random=0 LIMIT %1,%2")
#define Sql_f_pic_info_6values QString("INSERT INTO `f_pic_info`  (`f_pic_id`,`left`,`right`,`top`,`botton`,`data`) VALUES (%1,%2,%3,%4,%5,'%6')")
#define Sql_f_pic_update_withId QString("UPDATE `f_pic` set if_random=1 where id=%1")
#endif // DATABASESQL_H