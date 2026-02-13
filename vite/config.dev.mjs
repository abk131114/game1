import { defineConfig } from 'vite';
import staticAssetsPlugin from 'vite-static-assets-plugin';
import assetpackPlugin from './assetpackPlugin';

export default defineConfig({
  base: './',
  build: {
    rollupOptions: {
      output: {
        manualChunks: {
          phaser: ['phaser']
        }
      }
    },
  },
  plugins: [
    staticAssetsPlugin({
      directory: 'public',
      outputFile: 'src/static-assets.ts',
      // ignore: ['.DS_Store'],
      debounce: 200,
      enableDirectoryTypes: true,
      maxDirectoryDepth: 5,
      allowEmptyDirectories: true,
      addLeadingSlash: true,
    }),
    assetpackPlugin(),
  ],
  server: {
    port: 8080
  }
});
