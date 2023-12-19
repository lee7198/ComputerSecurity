'use client';

import React, { useState, useEffect, MouseEvent } from 'react';
import axios from 'axios';
import { useSocket } from '@/app/components/socketProvider';
import { useSearchParams } from 'next/navigation';
import Link from 'next/link';
import rsa, { decrypt } from 'js-crypto-rsa';
// import { decrypt } from './utils/rsa';
import { MSG, RSA_KEY } from '@/pages/type';
import MsgBox from './components/msgBox';

const ChatPage = () => {
  const searchParams = useSearchParams();
  const nickname = searchParams?.get('name');

  const [messages, setMessages] = useState<MSG[]>([]);
  const [currentMessage, setCurrentMessage] = useState('');
  const { socket, isConnected } = useSocket();

  const [keyPair, setKeyPair] = useState<RSA_KEY | undefined>(undefined);

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

  const sendMessage = async (e: MouseEvent<HTMLButtonElement>) => {
    e.preventDefault();
    if (!currentMessage) return;
    if (!keyPair) return;
    if (!keyPair.publicKey) return;
    if (!keyPair.privateKey) return;

    const utf8EncodeText = new TextEncoder();

    // encrypt
    rsa
      .encrypt(
        utf8EncodeText.encode(currentMessage),
        keyPair.publicKey,
        'SHA-256'
      )
      .then(async (encrypted) => {
        // 전송
        await axios.post('/api/chat', {
          nickname: nickname,
          content: currentMessage,
          ciphertext: encrypted,
          privateKey: keyPair.privateKey,
        });
      });

    setCurrentMessage('');
  };

  useEffect(() => {
    // key 생성
    axios.post('/api/keygen', { nickname: nickname }).then((res) => {
      setKeyPair(res.data);

      // 서버로부터 서버 공개키, 타 클라이언트 공개키, 타 클라이언트 증명값 받기
      axios.get(`/api/keygen?name=${nickname}`).then((res_) => {
        console.log(res_.data);
      });
    });

    // 언마운트 시 서버에 삭제 요청
    return () => {
      axios.delete(`/api/keygen?name=${nickname}`);
    };
  }, []);

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
            className={`h-3 w-3 animate-pulse rounded-full ${
              isConnected ? 'bg-green-500 ' : 'bg-yellow-400'
            }`}
          />
        </div>
        <div className="mb-2 h-[1px] w-full bg-zinc-200" />

        {/* 채팅 */}
        <div className="h-full overflow-y-scroll p-4 pt-0">
          {messages.map((message, index) => (
            <MsgBox message={message} nickname={nickname} key={index} />
          ))}
        </div>

        <div className="mt-2 h-[1px] w-full bg-zinc-200" />
        <form className="flex w-full items-center gap-2 p-2">
          <input
            type="text"
            value={currentMessage}
            onChange={(e) => setCurrentMessage(e.target.value)}
            placeholder="메세지을 입력하세요"
            className="w-full rounded-md px-3 py-1 text-sm outline-none"
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
