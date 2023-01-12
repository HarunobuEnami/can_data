# can_data

## Raspberry PiでのCAN FD通信

1. はじめに，/boot/config.txtをこのリポジトリのconfig.txt最後の3行を追加する．
2. 再起動
3. sudo ip link set up can0 type can  bitrate 500000 dbitrate 1000000 fd on のようにコマンドを叩いて，インタフェースを起動する(ipコマンドにオプションを付与したもの．順番に意味は linkをアップする(link set up)，インタフェース名(can0)，インタフェースタイプ(type can)，ボーレートの指定(bitrate 500000)，データビットレートの指定(dbitrate 1000000) fd on)
4. 私のリポジトリ内のomac_encryptなどを参考に通信してください
