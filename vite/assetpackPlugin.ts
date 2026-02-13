import { AssetPack, type AssetPackConfig } from '@assetpack/core';
import { texturePacker } from '@assetpack/core/texture-packer';
import { type Plugin, type ResolvedConfig } from 'vite';

export default function assetpackPlugin(): Plugin {
  const apConfig: AssetPackConfig = {
    entry: './resources',
    pipes: [
      texturePacker({
        addFrameNames: true,
        texturePacker: {
          padding: 2,
          nameStyle: 'relative',
          removeFileExtension: true,
        },
        resolutionOptions: {
          template: '@%%x',
          resolutions: { default: 1 }, // , low: 0.5 },
          fixedResolution: 'default',
          maximumTextureSize: 2048,
        }
      }),
    ],
  };

  let mode: ResolvedConfig['command'];
  let ap: AssetPack | undefined;

  return {
    name: 'vite-plugin-assetpack',

    configResolved(resolvedConfig) {
      mode = resolvedConfig.command;
      if (!resolvedConfig.publicDir) return;
      if (apConfig.output) return;
      const publicDir = resolvedConfig.publicDir.replace(process.cwd(), '');
      apConfig.output = `.${publicDir}/assets/`;
    },

    buildStart: async () => {
      if (mode === 'serve') {
        if (ap) return;
        ap = new AssetPack(apConfig);
        void ap.watch();
      } else {
        await new AssetPack(apConfig).run();
      }
    },

    buildEnd: async () => {
      if (ap) {
        await ap.stop();
        ap = undefined;
      }
    },
  };
}
