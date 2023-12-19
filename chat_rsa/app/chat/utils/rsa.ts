import rsa from 'js-crypto-rsa';

export async function decrypt(
  encrypted: Uint8Array,
  privateKey: JsonWebKey
): Promise<string> {
  console.log('decrypt start');

  await rsa.decrypt(encrypted, privateKey, 'SHA-256').then((decrypted) => {
    console.log(new TextDecoder().decode(decrypted));
  });

  return 'hello';
}
