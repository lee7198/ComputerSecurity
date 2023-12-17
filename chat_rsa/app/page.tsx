'use client';

import Link from 'next/link';
import { useState } from 'react';

export default function Home() {
  const [nickname, setNickname] = useState('');
  return (
    <div className="flex h-[100svh] w-full flex-col justify-center gap-4 px-4">
      <h1 className="text-3xl">RSA CHAT</h1>
      <div className="flex gap-4">
        <input
          type="text"
          value={nickname}
          onChange={(e) => setNickname(e.target.value)}
          placeholder="닉네임을 입력하세요"
          className="rounded-md border p-4 outline-none"
        />
        {nickname !== '' ? (
          <Link
            href={`/chat?name=${nickname}`}
            className="inline-block rounded-md bg-sky-500 p-4 text-white"
          >
            채팅방 이동하기
          </Link>
        ) : (
          <div className="inline-block rounded-md bg-zinc-400 p-4 text-white">
            채팅방 이동하기
          </div>
        )}
      </div>
    </div>
  );
}
