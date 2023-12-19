import { MSG } from '@/pages/type';
import React, { useEffect, useState } from 'react';
// import { decrypt } from '../utils/rsa';
import axios from 'axios';
import { decrypt } from 'js-crypto-rsa';

export default function MsgBox({
  message,
  nickname,
}: {
  message: MSG;
  nickname: string;
}) {
  const [plain, setPlain] = useState('');
  const decryptMessage = async (message: MSG) => {
    // return await axios
    //   .post('/api/rsa', {
    //     message: message,
    //   })
    //   .then((res) => res.data);

    if (message.ciphertext && message.privateKey)
      decrypt(message.ciphertext, message.privateKey).then((res) =>
        console.log(res)
      );
  };

  useEffect(() => {
    decryptMessage(message);
  }, []);

  return (
    <div
      className={
        'max-w-3/4 relative flex w-max flex-col gap-2 rounded-lg px-3 py-1 text-sm ' +
        (message.nickname !== nickname
          ? 'mb-6 bg-zinc-100'
          : 'mb-6 ml-auto bg-blue-400 text-white')
      }
    >
      <div className="absolute -top-4 left-0 text-xs">{message.nickname}</div>
      {message.nickname !== nickname
        ? message.ciphertext && message.privateKey && plain
        : message.content}
    </div>
  );
}
