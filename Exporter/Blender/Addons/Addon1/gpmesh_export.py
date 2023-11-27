# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTIBILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

bl_info = {
    "name" : "gpmeshExporter",
    "author" : "Zhengtu",
    "blender": (3,00,0),
    "category": "Export",
    "description": "Custom mesh format which is Vertice/Normal/UV and indice is triangle!",
}

import bpy

def write_gpmesh(context, filepath, vertFormat):
    # Get current object
    obj = bpy.context.active_object
    if obj.type == 'MESH':
        mesh = obj.data
        f = open(filepath, 'w', encoding='utf-8')
        f.write("{\n")
        f.write("\t\"version\":1,\n")
        f.write("\t\"vertexformat\":\"" + vertFormat + "\",\n")
        f.write("\t\"shader\":\"BasicMesh\",\n")
        
        # For now, only one texture
        # figure out the file name...
        texname = filepath.split("\\")[-1].split("/")[-1].split(".")[0]
        # Make it a png
        texname += ".png"
        f.write("\t\"textures\":[\n")
        f.write("\t\t\"" + texname + "\"\n")
        f.write("\t],\n")
        
        # specular power
        f.write("\t\"specularPower\":100,\n")
         
        # vertices
        # We have to create our own storage for because uvs are stored separately
        verts = [dict() for x in range(len(mesh.vertices))]
        for v in mesh.vertices:
            verts[v.index]["pos"] = v.co
            verts[v.index]["norm"] = v.normal 
            verts[v.index]["uv"] = mesh.uv_layers.active.data[0].uv       

        for l in mesh.loops:
            verts[l.vertex_index]["uv"] = mesh.uv_layers.active.data[l.index].uv
        
        f.write("\t\"vertices\":[\n")
        first = True
        for v in verts:
            if first:
                f.write("\t\t[")
                first = False
            else:
                f.write(",\n\t\t[")
            f.write("%f,%f,%f," % (v["pos"].x, v["pos"].y, v["pos"].z))
            f.write("%f,%f,%f," % (v["norm"].x, v["norm"].y, v["norm"].z))
            f.write("%f,%f" % (v["uv"].x, -1.0 * v["uv"].y))
            f.write("]")
        f.write("\n\t],\n")
        
        # indices
        f.write("\t\"indices\":[\n")
        first = True
        index = 0
        for p in mesh.polygons:
            index += 1
            poly_verts = list(p.vertices)
            if first:
                f.write("\t\t")
                first = False
            for i in range(1, len(poly_verts) - 1):
                f.write("[%d,%d,%d]" % (poly_verts[0], poly_verts[i], poly_verts[i+1]))
                if index != len(mesh.polygons) or (i != len(poly_verts) - 2):
                    f.write(",\n\t\t")
        f.write("\n\t]\n")
        
        f.write("}\n")
        f.close()
    else:
        raise ValueError("No mesh selected")

    return {'FINISHED'}


# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator

class ExportGPMesh(Operator, ExportHelper):
    """Export to Game Programming in C++ mesh format"""
    bl_idname = "export.gpmesh" # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "Export as gpmesh"

    # ExportHelper mixin class uses this
    filename_ext = ".gpmesh"

    filter_glob = StringProperty(
            default="*.gpmesh",
            options={'HIDDEN'},
            maxlen=255,  # Max internal buffer length, longer would be clamped.
            )
    
    vertFormat: EnumProperty(
            name="Vertex Format",
            description="Choose the vertex format",
            items=(('PosNormTex', "PosNormTex", "Position, normal, tex coord"),
                   ('PosNormSkinTex', "PosNormSkinTex", "Position, normal, skeleton index, skeleton weight, tex coord")),
            default='PosNormTex',
            )
    
    def execute(self, context):
        return write_gpmesh(context, self.filepath, self.vertFormat)


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportGPMesh.bl_idname, text="GPMesh Exporter (.gpmesh)")

def register():
    bpy.utils.register_class(ExportGPMesh)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)

def unregister():
    bpy.utils.unregister_class(ExportGPMesh)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
    register()

    # call
    bpy.ops.export.gpmesh('INVOKE_DEFAULT')