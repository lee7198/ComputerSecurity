import { Server as NetServer } from 'http';
import { NextApiRequest, NextApiResponse } from 'next';
import { Server as ServerIO } from 'socket.io';
import { Socket } from 'net';

export type NextApiResponseServerIo = NextApiResponse & {
  socket: Socket & {
    server: NetServer & {
      io: ServerIO;
    };
  };
};

const ioHandler = (req: NextApiRequest, res: NextApiResponseServerIo) => {
  if (res.socket.server.io) {
    console.log('이미 바인딩 되었습니다.');
  } else {
    console.log('서버-소켓 연결완료');
    const io = new ServerIO(res.socket.server);
    res.socket.server.io = io;
  }
  res.end();
};

export default ioHandler;
