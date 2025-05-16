import bpy

# ブレンダーに登録するアドオン情報
bl_info = {
    "name": "レベルエディタ",
    "author": "Wataru Akimoto",
    "version": (1, 0),
    "blender": (3, 3, 1),
    "location": "",
    "description": "レベルエディタ",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "category": ""
}

# メニュー項目描画
def draw_menu_manual(self, context):
    #self : 呼び出し元のクラスインスタンス。C++でいうthisポインタ
    #context : カーソルを合わせた時のポップアップのカスタマイズなどに使用

    # トップバーの「エディターメニュー」に項目(オペレータ)を追加
    self.layout.operator("wm.url_open_preset", text = "Manual", icon = 'HELP')

# アドオン有効化コールバック
def register():

    # Blenderにクラスを登録
    for cls in classes:
        bpy.utils.register_class(cls)
    
    # メニューに項目を追加
    bpy.types.TOPBAR_MT_editor_menus.append(TOPBAR_MT_my_menu.submenu)

    print("レベルエディタが有効化されました。")

# アドオン無効化コールバック
def unregister():

    # メニューから項目を削除
    bpy.types.TOPBAR_MT_editor_menus.remove(TOPBAR_MT_my_menu.submenu)

    # Blenderにクラスを登録
    for cls in classes:
        bpy.utils.unregister_class(cls)
    
    print("レベルエディタが無効化されました。")

# テスト実行用コード
if __name__ == "__main__":
    register()

# トップバー拡張メニュー
class TOPBAR_MT_my_menu(bpy.types.Menu):
    # Blenderがクラスを識別するための固有の文字列
    bl_idname = "TOPBAR_MT_my_menu"
    # メニューのラベルとして表示される文字列
    bl_label = "MyMenu"
    # 著者表示用の文字列
    bl_description = "拡張メニュー by " + bl_info["author"]

    # サブメニューの描画
    def draw(self, context):

        # トップバーの「エディターメニュー」に項目(オペレータ)を追加
        self.layout.operator(MYADDON_OT_stretch_vertex.bl_idname, text = MYADDON_OT_stretch_vertex.bl_label)
        # 区切り線
        self.layout.separator()
        # トップバーの「エディターメニュー」に項目(オペレータ)を追加
        self.layout.operator(MYADDON_OT_create_ico_sphere.bl_idname, text = MYADDON_OT_create_ico_sphere.bl_label)
         # 区切り線
        self.layout.separator()
        # トップバーの「エディターメニュー」に項目(オペレータ)を追加
        self.layout.operator("wm.url_open_preset", text = "Manual", icon = 'HELP')
    
    # 既存のメニューにサブメニューを追加
    def submenu(self, context):

        # ID指定でサブメニューを追加
        self.layout.menu(TOPBAR_MT_my_menu.bl_idname)

# オペレータ 頂点を伸ばす
class MYADDON_OT_stretch_vertex(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_stretch_vertex"
    bl_label = "頂点を伸ばす"
    bl_description = "頂点座標を引っ張って伸ばします"
    #リドゥ、アンドゥ可能オプション
    bl_options = {'REGISTER', 'UNDO'}

    #メニューを実行したときに呼ばれるコールバック関数
    def execute(self, context):
        bpy.data.objects["Cube"].data.vertices[0].co.x +=1.0
        print("頂点を伸ばしました。")

        #オペレータの命令終了を通知
        return {'FINISHED'}
    
# オペレータ ICO球生成
class MYADDON_OT_create_ico_sphere(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_create_objects"
    bl_label = "ICO球生成"
    bl_description = "ICO球を生成します"
    #リドゥ、アンドゥ可能オプション
    bl_options = {'REGISTER', 'UNDO'}

    #メニューを実行したときに呼ばれるコールバック関数
    def execute(self, context):
        bpy.ops.mesh.primitive_ico_sphere_add()
        print("ICO球を生成しました。")

        #オペレータの命令終了を通知
        return {'FINISHED'}

# Blenderに登録するクラスリスト
classes = (
    MYADDON_OT_stretch_vertex,
    MYADDON_OT_create_ico_sphere,
    TOPBAR_MT_my_menu,
)