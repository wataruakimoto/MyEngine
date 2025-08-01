import bpy
import os
import bpy.ops

#データテーブル
class SpawnNames():

    #インデックス
    PROTOTYPE = 0 #プロトタイプのオブジェクト名
    INSTANCE = 1 #量産時のオブジェクト名
    FILENAME = 2 #リソースファイル名

    names = {}

    #names["キー"] = (プロトタイプのオブジェクト名、 量産時のオブジェクト名、 リソースファイル名)
    names["Player"] = ("PrototypePlayerSpawn", "PlayerSpawn", "Player/player.obj")
    names["Enemy"] = ("PrototypeEnemySpawn", "EnemySpawn", "Enemy/enemy.obj")

#オペレータ 出現ポイントのシンボルを読み込む
class MYADDON_OT_load_symbol(bpy.types.Operator):

    bl_idname = "myaddon.myaddon_ot_load_symbol"
    bl_label = "SpawnPointSymbol 読み込み"
    bl_description = "出現ポイントのシンボルを読み込みます"

    #リドゥ、アンドゥ可能オプション
    bl_options = {'REGISTER', 'UNDO'}

    #1モデルの読み込み処理
    def load_obj(self, type):

        print("出現ポイントのシンボルを読み込みます")

        #重複ロード禁止
        spawn_object = bpy.data.objects.get(SpawnNames.names[type][SpawnNames.PROTOTYPE])
        if spawn_object is not None:
            return {'CANCELLED'}

        #スクリプトが配置されているディレクトリの名前を取得する
        addon_directory = os.path.dirname(__file__)

        #合成してモデルファイルのフルパスを得る
        full_path = os.path.join(addon_directory, SpawnNames.names[type][SpawnNames.FILENAME])

        #オブジェクトをインポート
        bpy.ops.wm.obj_import('EXEC_DEFAULT', filepath = full_path,
            display_type = 'THUMBNAIL', forward_axis = 'Z', up_axis = 'Y')
        
        #回転を適用
        bpy.ops.object.transform_apply(location = False, rotation = True,
            scale = False, properties = False, isolate_users = False)
        
        #アクティブなオブジェクトを取得
        object = bpy.context.active_object

        #オブジェクト名を変更
        object.name = SpawnNames.names[type][SpawnNames.FILENAME]

        #オブジェクトの種類を設定
        object["type"] = SpawnNames.names[type][SpawnNames.INSTANCE]

        #メモリ上にはおいておくがシーンから外す
        bpy.context.collection.objects.unlink(object)

        #オペレータの命令終了を通知
        return {'FINISHED'}
    
    #メニューを実行したときに呼ばれるコールバック関数
    def execute(self, context):

        #Playerオブジェクト読み込み
        self.load_obj("Player")
        #Enemyオブジェクト読み込み
        self.load_obj("Enemy")

        return {'FINISHED'}
    
#オペレータ 出現ポイントのシンボルを作成・配置する
class MYADDON_OT_generate_symbol(bpy.types.Operator):

    bl_idname = "myaddon.myaddon_ot_generate_symbol"
    bl_label = "SpawnPointSymbol 作成"
    bl_description = "出現ポイントのシンボルを作成します"

    #リドゥ、アンドゥ可能オプション
    bl_options = {'REGISTER', 'UNDO'}

    #プロパティ(引数として渡せる)
    type: bpy.props.StringProperty(name = "Type", default = "Player")

    #メニューを実行したときに呼ばれるコールバック関数
    def execute(self, context):

        #読み込み済みのコピー元オブジェクトをを検索
        spawn_object = bpy.data.objects.get(SpawnNames.names[self.type][SpawnNames.PROTOTYPE])

        #まだ読み込んでいない場合
        if spawn_object is None:
            #読み込み済みオペレータを実行する
            bpy.ops.myaddon.myaddon_ot_load_symbol('EXEC_DEFAULT')
            # 再検索。今度は見つかるはず
            spawn_object = bpy.data.objects.get(SpawnNames.names[self.type][SpawnNames.PROTOTYPE])

        print("出現ポイントのシンボルを作成します")

        #Blenderでの選択を解除する
        bpy.ops.object.select_all(action='DESELECT')

        #複製元の非表示オブジェクトを複製する
        object = spawn_object.copy()

        #複製したオブジェクトを現在のシーンにリンク(出現させる)
        bpy.context.collection.objects.link(object)

        #オブジェクト名を変更
        object.name = SpawnNames.names[self.type][SpawnNames.INSTANCE]

        return {'FINISHED'}
    
#オペレータ 自キャラ専用
class MYADDON_OT_generate_player_symbol(bpy.types.Operator):

    bl_idname = "myaddon.myaddon_ot_generate_player_symbol"
    bl_label = "PlayerSpawnSymbol 作成"
    bl_description = "自キャラの出現ポイントのシンボルを作成します"

    #メニューを実行したときに呼ばれるコールバック関数
    def execute(self, context):

        bpy.ops.myaddon.myaddon_ot_generate_symbol('EXEC_DEFAULT', type = "Player")

        return {'FINISHED'}
    
#オペレータ 敵キャラ専用
class MYADDON_OT_generate_enemy_symbol(bpy.types.Operator):

    bl_idname = "myaddon.myaddon_ot_generate_enemy_symbol"
    bl_label = "EnemySpawnSymbol 作成"
    bl_description = "敵キャラの出現ポイントのシンボルを作成します"

    #メニューを実行したときに呼ばれるコールバック関数
    def execute(self, context):

        bpy.ops.myaddon.myaddon_ot_generate_symbol('EXEC_DEFAULT', type = "Enemy")

        return {'FINISHED'}