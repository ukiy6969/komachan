{
  'make_global_settings': [
    ['CXX','/usr/bin/clang++'],
    ['LINK','/usr/bin/clang++'],
  ],
  'targets': [
    {
      'target_name': 'komachan',
      'sources': [
        'src/ccfiles/komachan.cc',
        'src/ccfiles/game.cc',
        'src/ccfiles/board.cc',
        'src/ccfiles/search.cc',
        'src/ccfiles/io.cc'
      ],
      'include_dirs': [
        "<!(node -e \"require('nan')\")"
      ],
      'conditions': [
        [ 'OS=="mac"', {
          'xcode_settings': {
            'OTHER_CPLUSPLUSFLAGS' : ['-stdlib=libc++'],
            'MACOSX_DEPLOYMENT_TARGET': '10.7',
            },
          }
        ],
      ],
      'cflags' : [
        '-stdlib=libc++'
      ],
    }
  ]
}
