#ifndef __SERVER_HPP__
#define __SERVER_HPP__

namespace Jason
{
    class ServerBase {

    }

    class WorldServer : public ServerBase
    {
        void Init();
        void Start();
        void Close();
        void SendMessage();
        void BoardcastMessage();
    }
}

#endif