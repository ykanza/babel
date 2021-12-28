#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <vector>
#include <client/src/Codec/Opus.hpp>

enum TYPE
{
    DEFAULT,
    LOGIN_REQ,
    LOGIN_RES,
    CALL_REQ,
    CALL_RES,
    MSG_REQ,
    MSG_RES,
    AUDIO_REQ,
    AUDIO_RES,
    LIST_REQ,
    LIST_RES,
    HANG_REQ,
    HANG_RES
};

enum CODE
{
    SUCCESS,
    CLIENT_DISCONNECTED,
};

/*****************************
 *      Package contained    *
 * **************************/

/*      subRequest     */

struct userReqCall
{
    int idCalled;
    int idCaller;
    int accept;
    int whoCalled; /* 0 = C1 -> S
                      1 = C2 -> S */
};

struct userReqAudio
{
    int toID;
    EncodedPacket encodedPacket;
};

struct userReqHang
{
    int idToHang;
};

struct userReq /* Request */
{
    userReqCall call;
    userReqAudio audio;
    userReqHang hang;
};

/*      subResponse        */

struct infoClient {
    int id;
    char ip[15];
    int port;
};

struct userResCall
{
    infoClient infoCaller;
    infoClient infoCalled;
    int accept;    /* 0 = refuse, 1 = accept */
    int whoCalled; /* 0 = C1 -> S     1 = C2 -> S */
};

struct userResAudio
{
    int fromID;
    EncodedPacket encodedPacket;
};

struct userResList
{
    int userList[50];
    int nbUser;
};

struct userRes /* Response */
{
    userResCall call;
    userResAudio audio;
    userResList list;
};

/*****************************
 *      Package Protocol     *
 * **************************/

struct Header
{
    TYPE type;
};

struct Body
{
    union protocol
    {
        userReq req;
        userRes res;
    } pkg;
};

struct Package {
    Header header;
    Body body;
};

#endif