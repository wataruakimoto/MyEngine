import bpy
import os
import bpy.ops

#オペレータ 出現ポイントのシンボルを読み込む
class MYADDON_OT_load_symbol(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_load_symbol"
    bl_label = "SpawnPointSymbol 読み込み"
    bl_description = "出現ポイントのシンボルを読み込みます"

    #リドゥ、アンドゥ可能オプション
    bl_options = {'REGISTER', 'UNDO'}

    prototype_object_name = "PrototypePlayerSpawn"
    object_name = "PlayerSpawn"

    #メニューを実行したときに呼ばれるコールバック関数
    def execute(self, context):

        print("出現ポイントのシンボルを読み込みます")

        #スクリプトが配置されているディレクトリの名前を取得する
        addon_directory = os.path.dirname(__file__)
        
        #ディレクトリからのモデルファイルの相対パスを記述
        relative_path = "player/player.obj"

        #合成してモデルファイルのフルパスを得る
        full_path = os.path.join(addon_directory, relative_path)

        #オブジェクトをインポート
        bpy.ops.wm.obj_import('EXEC_DEFAULT', filepath = full_path,
            display_type = 'THUMBNAIL', forword_axis = 'Z', up_axis = 'Z')
        
        #回転を適用
        bpy.ops.object.transform_apply(location = False, rotation = True,
            scale = False, properties = False, isolate_users = False)
        
        #アクティブなオブジェクトを取得
        object.name = MYADDON_OT_load_symbol.prototype_object_name

        #オブジェクトの種類を設定
        object["type"] = MYADDON_OT_load_symbol.object_name

        #オペレータの命令終了を通知
        return {'FINISHED'}