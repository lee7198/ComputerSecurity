'use client';

import React, { useState, useEffect } from 'react';
import axios from 'axios';
import { useSocket } from '@/app/components/socketProvider';
import { useSearchParams } from 'next/navigation';
import Link from 'next/link';

interface message {
  nickname: string;
  content: string;
}

const ChatPage = () => {
  const searchParams = useSearchParams();
  const nickname = searchParams?.get('name');

  const [messages, setMessages] = useState<message[]>([]);
  const [currentMessage, setCurrentMessage] = useState('');
  const { socket, isConnected } = useSocket();

  useEffect(() => {
    if (!socket) {
      return;
    }

    socket.on('message', (data: any) => {
      setMessages((messages) => [...messages, ...[data]]);
    });

    return () => {
      socket.off('message');
    };
  }, [socket, messages]);

  const sendMessage = async (e: React.MouseEvent<HTMLButtonElement>) => {
    e.preventDefault();
    await axios.post('/api/chat', {
      nickname: nickname,
      content: currentMessage,
    });
    setCurrentMessage('');
  };
  if (!nickname)
    return (
      <div className="flex h-[100svh] w-full flex-col items-center justify-center gap-4">
        <div className="text-xl">닉네임을 설정해주세요</div>
        <Link href="/" className="rounded-md bg-sky-500 px-4 py-2 text-white">
          되돌아가기
        </Link>
      </div>
    );

  return (
    <div className="flex h-[100svh] w-full items-center justify-center">
      <div className="relative flex h-[80svh] w-80 flex-col rounded-lg border">
        {/* 상태바 */}
        <div className="flex items-center justify-between px-4 py-2 capitalize">
          <div>{nickname}</div>
          <div
            className={`h-3 w-3 rounded-full ${
              isConnected ? 'bg-green-500' : 'bg-yellow-400'
            }`}
          />
        </div>

        {/* 채팅 */}
        <div className="h-full overflow-y-scroll p-4 pt-0">
          {messages.map((message, index) => (
            <div
              key={index}
              className={
                'max-w-3/4 relative flex w-max flex-col gap-2 rounded-lg px-3 py-1 text-sm ' +
                (message.nickname !== nickname
                  ? 'mb-6 bg-zinc-100'
                  : 'mb-6 ml-auto bg-blue-400 text-white')
              }
            >
              <div className="absolute -top-4 left-0 text-xs">
                {message.nickname}
              </div>
              {message.content}
            </div>
          ))}
        </div>

        <form className="flex w-full items-center gap-2 p-2">
          <input
            type="text"
            value={currentMessage}
            onChange={(e) => setCurrentMessage(e.target.value)}
            placeholder="메세지을 입력하세요"
            className="w-full rounded-md border px-3 py-1 text-sm outline-none"
          ></input>
          <button
            type="submit"
            onClick={(e) => sendMessage(e)}
            className="shrink-0 rounded-md border-none bg-sky-500 px-3 py-1 text-sm text-white outline-none"
          >
            전송
          </button>
        </form>
      </div>
    </div>
  );
};

export default ChatPage;
