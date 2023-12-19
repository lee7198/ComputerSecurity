export interface MSG {
  nickname: string;
  content: string;
  ciphertext?: Uint8Array;
  privateKey?: JsonWebKey;
}

export interface message {
  nickname: string;
  content: string;
}

export interface RSA_KEY {
  publicKey: JsonWebKey;
  privateKey: JsonWebKey;
}
