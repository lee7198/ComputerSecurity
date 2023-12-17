import type { Metadata } from 'next';
import { Inter } from 'next/font/google';
import { SocketProvider } from './components/socketProvider';
import './globals.css';

const inter = Inter({ subsets: ['latin'] });

export const metadata: Metadata = {
  title: 'RSA CHAT',
};

export default function RootLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  return (
    <html lang="ko">
      <SocketProvider>
        <body
          className={`${inter.className} mx-auto max-w-screen-sm`}
          suppressHydrationWarning={process.env.NODE_ENV === 'development'}
        >
          {children}
        </body>
      </SocketProvider>
    </html>
  );
}
