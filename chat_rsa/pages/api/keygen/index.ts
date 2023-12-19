import { NextApiRequest, NextApiResponse } from 'next';
import rsa from 'js-crypto-rsa';
import { RSA_KEY } from '@/pages/type';
import { JsonWebKeyPair } from 'js-crypto-rsa/dist/typedef';
import NodeRSA from 'node-rsa';

interface USER_KEY {
  nickname: string;
  publicKey: string;
  privateKey: string;
}

const serverKey = new NodeRSA({ b: 512 });
// 유저 키 목록
const users = new Map<string, USER_KEY>();

export default async (req: NextApiRequest, res: NextApiResponse) => {
  if (req.method === 'POST') {
    const { nickname }: { nickname: string } = req.body;

    // 유저 미등록 시
    if (!users.get(nickname)) {
      const key = new NodeRSA({ b: 512 });

      users.set(nickname, {
        publicKey: key.exportKey('public'),
        privateKey: key.exportKey('private'),
        nickname,
      });
    }
    res.status(200).json(users.get(nickname));
  } else if (req.method === 'DELETE') {
    // 유저 삭제
    const name = req.query.name as string;
    users.delete(name);
  } else if (req.method === 'GET') {
    // 유저 공개 키 반환
    const name = req.query.name as string;
    users.forEach((user) => {
      if (user.nickname !== name) {
        const key = new NodeRSA(user.privateKey);
        // 서명값 전달
        res.status(201).json({
          serverKey: serverKey.exportKey('public'),
          publicKey: user.publicKey,
          signature: key.sign('', 'utf8'),
        });
        return;
      }
    });
    res.status(201).json('');
  }

  // res.status(200).json();
};
export const config = {
  api: {
    externalResolver: true,
  },
};
