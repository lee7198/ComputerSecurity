import { MSG } from '@/pages/type';
import { NextApiRequest } from 'next';
import rsa from 'js-crypto-rsa';

export default async (req: NextApiRequest, res: any) => {
  if (req.method === 'POST') {
    const message: MSG = req.body.message;

    if (message.ciphertext && message.privateKey)
      await rsa
        .decrypt(message.ciphertext, message.privateKey, 'SHA-256')
        .then((decrypted) => {
          res.status(201).json(new TextDecoder().decode(decrypted));
        });
  }
};
