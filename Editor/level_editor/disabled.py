import bpy

#オペレータ カスタムプロパティ{'disabled'}追加
class MYADDON_OT_add_disabled(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_disabled"
    bl_label = "Disabled 追加"
    bl_description = "有効にするとゲームに表示されません"

    #リドゥ、アンドゥ可能オプション
    bl_options = {'REGISTER', 'UNDO'}

    #メニューを実行したときに呼ばれるコールバック関数
    def execute(self, context):

        #['disabled']カスタムプロパティを追加
        context.object['disabled'] = True

        #オペレータの命令終了を通知
        return {'FINISHED'}
    
#パネル 無効オプション
class OBJECT_PT_set_export_disabled(bpy.types.Panel):
    """"""
    bl_idname = "OBJECT_PT_set_export_disabled"
    bl_label = "Disabled"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    #サブメニューの描画
    def draw(self, context):
        
        #パネルに項目を追加
        if "disabled"in context.object:
           #すでにプロパティがあれば、プロパティを追加
            self.layout.prop(context.object, '["disabled"]', text = self.bl_label)
        else:
            #プロパティがなければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_disabled.bl_idname)