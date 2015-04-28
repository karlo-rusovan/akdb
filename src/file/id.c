/**
   @file id.c Provides functions for creating id of objects
*/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */

#include "id.h"

/**
 * @author Saša Vukšić, updated by Mislav Čakarić, changed by Mario Peroković, now uses Ak_update_row, updated by Nenad Makar
 * @brief Function for getting unique ID for any object, stored in sequence
 * @return objectID
 */
int AK_get_id() {
    int obj_id = 0;
    int current_value;
    AK_PRO;
    struct list_node *row_root = (struct list_node *) AK_malloc(sizeof (struct list_node));
    Ak_Init_L3(&row_root); 
    
    int num_rec = AK_get_num_records("AK_sequence");

    if(num_rec == 1) {
    	struct list_node *row = AK_get_row(0, "AK_sequence");
		struct list_node *attribute = Ak_GetNth_L2(3, row);
		memcpy(&current_value, &attribute->data, attribute->size);
        current_value++;
		Ak_Insert_New_Element_For_Update(TYPE_INT, &obj_id, "AK_sequence", "obj_id", row_root, SEARCH_CONSTRAINT);
        Ak_Insert_New_Element_For_Update(TYPE_INT, &current_value, "AK_sequence", "current_value", row_root, NEW_VALUE);
        int result = Ak_update_row(row_root);
        Ak_DeleteAll_L3(&row_root);
        AK_free(row_root);

        if(result != EXIT_SUCCESS){
            AK_EPI;
            return EXIT_ERROR;
        }

        AK_EPI;
        return current_value;
    }
    else
    {
		Ak_Insert_New_Element(TYPE_INT, &obj_id, "AK_sequence", "obj_id", row_root);
		Ak_Insert_New_Element(TYPE_VARCHAR, "objectID", "AK_sequence", "name", row_root);
		current_value = ID_START_VALUE;
		Ak_Insert_New_Element(TYPE_INT, &current_value, "AK_sequence", "current_value", row_root);
		int increment = 1;
		Ak_Insert_New_Element(TYPE_INT, &increment, "AK_sequence", "increment", row_root);
		Ak_insert_row(row_root);
		Ak_DeleteAll_L3(&row_root);
		AK_free(row_root);
		AK_EPI;
		return current_value;
    }
}

/**
 * @author Mislav Čakarić, updated by Nenad Makar
 * @brief  Function for testing getting ID's
 * @return No retun value
 */
void Ak_id_test() {
    AK_PRO;
    printf("\nCurrent value of objectID (depends on number of AK_get_id() calls (when objects are created...) before call of Ak_id_test()):\n\n");
    AK_print_table("AK_sequence");
    AK_get_id();
    printf("\nIncremented value of objectID:\n\n");
    AK_print_table("AK_sequence");
    AK_get_id();
    printf("\nIncremented value of objectID:\n\n");
    AK_print_table("AK_sequence");
    printf("\nTest succeeded.\nIt's clear that objectID was created after first call of AK_get_id() function (when ./akdb test created first DB object) then incremented after other calls.");
    AK_EPI;
}