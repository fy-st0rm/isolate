#ifndef __ISO_ECS_H__
#define __ISO_ECS_H__

#include "iso_util/iso_includes.h"
#include "iso_util/iso_defines.h"
#include "iso_util/iso_memory.h"
#include "iso_math.h"

/*
 * Table structure
 * 
 * iso_comp_table = {
 *                    iso_comp_entry
 *                         |
 *                        \/
 *	"component_1": { [ent1, comp1], [ent2, comp2], [ent3, comp3] ... }   <== iso_comp_record
 *	"component_2": { [ent1, comp1], [ent2, comp2], [ent3, comp3] ... }
 * }
 */

/* =======================
 * Entity
 * ======================= */

/*
 * @brief Enum to determine if the entity slot is OCCUPIED or FREE
 */

typedef enum {
	FREE,
	OCCUPIED
} iso_entity_slot_state;


/*
 * @brief Iso Entity type definition
 */

typedef u32 iso_entity;


/* =======================
 * Component Entry
 * ======================= */


/*
 * @brief Component struct that stores entity id and component data
 * @mem ent  = Entity id
 * @mem data = Component data
 */

typedef struct {
	iso_entity ent;
	void* data;
} iso_comp_entry;


/*
 * @brief Function to create component entry
 * @param ent  = iso_entity
 * @param data = Component data
 * @return Returns pointer to iso_comp_entry struct
 */

static iso_comp_entry* iso_comp_entry_new(iso_entity ent, void* data) {
	iso_comp_entry* entry = iso_alloc(sizeof(iso_comp_entry));
	entry->ent = ent;
	entry->data = data;
	return entry;
}


/*
 * @brief Function to delete iso_comp_entry
 * @param entry = Pointer to iso_comp_entry struct
 */

static void iso_comp_entry_delete(iso_comp_entry* entry) {
	iso_free(entry->data);
	iso_free(entry);
}


/* =======================
 * Component Record
 * ======================= */


/*
 * @brief Struct that holds a multiple entries of a single component type
 * @mem name          = Name of the component
 * @mem entry_cnt     = No of entry created
 * @mem max_entry_cnt = Max amount of entry that can be created
 * @mem entries       = Array of iso_comp_entry
 */

typedef struct {
	char* name;
	u32   entry_cnt;
	u32   max_entry_cnt;
	iso_comp_entry** entries;
} iso_comp_record;


/*
 * @brief Function to create a new iso_comp_record
 * @param name          = Name of the record
 * @param max_entry_cnt = Max amount of entry to support
 * @return Returns pointer to iso_comp_record struct
 */

static iso_comp_record* iso_comp_record_new(char* name, u32 max_entry_cnt) {
	iso_comp_record* rec = iso_alloc(sizeof(iso_comp_record));

	// Initializing variables
	rec->entries = iso_alloc(sizeof(iso_comp_entry) * max_entry_cnt);
	rec->entry_cnt = 0;
	rec->max_entry_cnt = max_entry_cnt;

	rec->name = iso_alloc(strlen(name));
	strcpy(rec->name, name);

	return rec;
}


/*
 * @brief Function to delete the iso_comp_record
 * @param rec = Pointer to iso_comp_record struct
 */

static void iso_comp_record_delete(iso_comp_record* rec) {
	for (u32 i = 0; i < rec->max_entry_cnt; i++) {
		iso_comp_entry* entry = rec->entries[i];
		if (entry) iso_comp_entry_delete(entry);
	}
	iso_free(rec->name);
	iso_free(rec->entries);
	iso_free(rec);
}


/*
 * @brief Function to search an entry in record according to entity id
 * @param rec = Pointer to iso_comp_record
 * @param ent = iso_entity
 * @return Returns True if found else False
 */

static b8 iso_comp_record_search(iso_comp_record* rec, iso_entity ent) {
	iso_assert(ent < rec->max_entry_cnt, "Tried to access entity of slot `%d` in record of max slot `%d`.\n", ent, rec->max_entry_cnt);

	iso_comp_entry* entry = rec->entries[ent];
	if (entry) return true;
	return false;
}


/*
 * @brief Function to add a new entry in the component record
 * @param rec  = Pointer to iso_comp_record
 * @param ent  = iso_entity
 * @param data = Component data
 * @return Returns pointer to iso_comp_entry struct
 */

static iso_comp_entry* iso_comp_record_add_entry(iso_comp_record* rec, iso_entity ent, void* data) {
	iso_assert(ent < rec->max_entry_cnt, "Tried to access entity of slot `%d` in record of max slot `%d`.\n", ent, rec->max_entry_cnt);
	iso_assert(rec->entry_cnt < rec->max_entry_cnt, "Component record is full. Cannot add entry to entity `%d`.\n", ent);

	iso_comp_entry* entry = iso_comp_entry_new(ent, data);
	rec->entries[ent] = entry;
	rec->entry_cnt++;

	return entry;
}


/*
 * @brief Function to get the entry from record according to the entity id
 * @param rec = Pointer to the iso_comp_record struct
 * @param ent = iso_entity
 * @return Returns pointer to iso_comp_entry struct
 */

static iso_comp_entry* iso_comp_record_get_entry(iso_comp_record* rec, iso_entity ent) {
	iso_assert(ent < rec->max_entry_cnt, "Tried to access entity of slot `%d` in record of max slot `%d`.\n", ent, rec->max_entry_cnt);

	iso_comp_entry* entry = rec->entries[ent];
	if (entry) return entry;
	return NULL;
}


/*
 * @brief Function to remove entry from the record
 * @param rec = Pointer to the iso_comp_record
 * @param ent = iso_entity
 */

static void iso_comp_record_remove_entry(iso_comp_record* rec, iso_entity ent) {
	iso_assert(ent < rec->max_entry_cnt, "Tried to access entity of slot `%d` in record of max slot `%d`.\n", ent, rec->max_entry_cnt);

	iso_comp_entry* entry = rec->entries[ent];
	iso_comp_entry_delete(entry);

	rec->entries[ent] = NULL;
	rec->entry_cnt--;
}

/* =======================
 * Component Table
 * ======================= */


/*
 * @brief Structure to hold multiple component records
 * @mem record_cnt     = Total amount of records created
 * @mem max_record_cnt = Max amount of record that can be created
 * @mem records        = Array of iso_comp_record
 */

typedef struct {
	u32 record_cnt;
	u32 max_record_cnt;
	iso_comp_record** records;
} iso_comp_table;


/*
 * @brief Function to create new iso_comp_table
 * @param max_record_cnt = Maximum amount of record to be created
 * @return Returns pointer to iso_comp_table struct
 */

static iso_comp_table* iso_comp_table_new(u32 max_record_cnt) {
	iso_comp_table* table = iso_alloc(sizeof(iso_comp_table));
	table->record_cnt = 0;
	table->max_record_cnt = max_record_cnt;
	table->records = iso_alloc(sizeof(iso_comp_record) * max_record_cnt);
	return table;
}


/*
 * @brief Function to delete iso_comp_table
 * @param table = Pointer to the iso_comp_table
 */

static void iso_comp_table_delete(iso_comp_table* table) {
	for (u32 i = 0; i < table->record_cnt; i++) {
		iso_comp_record* rec = table->records[i];
		iso_comp_record_delete(rec);
	}
	iso_free(table->records);
	iso_free(table);
}


/*
 * @brief Function to search for component in iso_comp_table
 * @param table = Pointer to the iso_comp_table
 * @param name  = Name of the component to be searched
 * @return Returns True if found else False
 */

#define iso_comp_table_search(table, comp) __iso_comp_table_search(table, #comp)

static b8 __iso_comp_table_search(iso_comp_table* table, char* name) {
	for (u32 i = 0; i < table->record_cnt; i++) {
		iso_comp_record* rec = table->records[i];
		if (strcmp(name, rec->name) == 0) {
			return true;
		}
	}
	return false;
}


/*
 * @brief Function to add new component record to the table
 * @param table = Pointer to the iso_comp_table
 * @param name  = Name of the component to be added
 */

#define iso_comp_table_add_record(table, comp) __iso_comp_table_add_record(table, #comp)

static void __iso_comp_table_add_record(iso_comp_table* table, char* name) {
	iso_assert(table->record_cnt < table->max_record_cnt, "Component table is full. Cant create new record `%s`.\n", name);
	table->records[table->record_cnt++] = iso_comp_record_new(name, table->max_record_cnt);
}


/*
 * @brief Function to get the component record from the table
 * @param table = Pointer to the iso_comp_table
 * @param name  = Name of the component to get
 * @return Returns pointer to iso_comp_record if found else NULL is returned
 */

#define iso_comp_table_get_record(table, comp) __iso_comp_table_get_record(table, #comp)

static iso_comp_record* __iso_comp_table_get_record(iso_comp_table* table, char* name) {
	for (u32 i = 0; i < table->record_cnt; i++) {
		iso_comp_record* rec = table->records[i];
		if (strcmp(name, rec->name) == 0) {
			return rec;
		}
	}
	return NULL;
}


/* =======================
 * Entity Component System
 * ======================= */


/*
 * @brief Structure that holds entire component in the system
 * @mem entity_cnt     = No of entity created
 * @mem max_entity_cnt = Max no of entity that can be created
 * @mem slots          = Slots for entity (iso_entity_slot_state)
 * @mem table          = Pointer to the iso_comp_table
 */

typedef struct {
	u32 entity_cnt;
	u32 max_entity_cnt;
	iso_entity_slot_state* slots;
	iso_comp_table* table;
} iso_ecs;


/*
 * @brief Function to create a new ecs
 * @param max_entity_cnt = Maximum amount of entity to be handled by ecs
 * @return Returns pointer to iso_ecs struct
 */

static iso_ecs* iso_ecs_new(u32 max_entity_cnt) {
	iso_ecs* ecs = iso_alloc(sizeof(iso_ecs));
	ecs->entity_cnt = 0;
	ecs->max_entity_cnt = max_entity_cnt;

	// Allocating entity slots
	ecs->slots = iso_alloc(sizeof(iso_entity_slot_state) * max_entity_cnt);
	memset(ecs->slots, FREE, sizeof(iso_entity_slot_state) * max_entity_cnt);

	// Creating table
	ecs->table = iso_comp_table_new(max_entity_cnt);

	return ecs;
}


/*
 * @brief Function to delete iso_ecs
 * @param ecs = Pointer to iso_ecs struct
 */

static void iso_ecs_delete(iso_ecs* ecs) {
	//TODO: Fix this with proper log
	if (ecs->entity_cnt) {
		printf("Not all the entities are deleted.\n");
	}

	iso_comp_table_delete(ecs->table);
	iso_free(ecs->slots);
	iso_free(ecs);
}


/* =======================
 * Entity Commands
 * ======================= */


/*
 * @brief Function to create new entity
 * @param ecs = Pointer to iso_ecs
 * @return Returns a iso_entity type which is a entity id
 */

static iso_entity iso_entity_new(iso_ecs* ecs) {
	iso_assert(ecs->entity_cnt < ecs->max_entity_cnt, "Entity slots are full.\n");
	ecs->entity_cnt++;

	// Generating random entity id
	iso_entity id = iso_rand_range(0, ecs->max_entity_cnt);
	do {
		id = iso_rand_range(0, ecs->max_entity_cnt);
	} while (ecs->slots[id] != FREE);

	// Making the slot occupied
	ecs->slots[id] = OCCUPIED;
	return id;
}


/*
 * @brief Function to delete an entity
 * @param ecs = Pointer to the iso_ecs
 * @param ent = iso_entity id
 */

static void iso_entity_delete(iso_ecs* ecs, iso_entity ent) {
	iso_assert(ecs->slots[ent] == OCCUPIED, "Entity `%d` doesnt exists.\n", ent);

	// Searching for components that entity has
	for (u32 i = 0; i < ecs->table->record_cnt; i++) {
		iso_comp_record* rec = ecs->table->records[i];
		if (iso_comp_record_search(rec, ent)) {
			iso_comp_record_remove_entry(rec, ent);
		}
	}

	// Reseting the slot
	ecs->slots[ent] = FREE;
	ecs->entity_cnt--;
}


/*
 * @brief Internal function to add component to entity
 * @param ecs  = Pointer to iso_ecs
 * @param ent  = iso_entity id
 * @param name = Name of the component
 * @param data = Data of the component
 */

static void* __iso_entity_add_component(iso_ecs* ecs, iso_entity ent, char* name, void* data) {
	iso_assert(ecs->slots[ent] == OCCUPIED, "Entity `%d` doesnt exists.\n", ent);

	if (!__iso_comp_table_search(ecs->table, name)) {
		__iso_comp_table_add_record(ecs->table, name);
	}

	iso_comp_record* rec  = __iso_comp_table_get_record(ecs->table, name);
	iso_comp_entry* entry = iso_comp_record_add_entry(rec, ent, data);

	iso_assert(entry, "Failed to add component `%s` in entity `%d`.\n", name, ent);
	return entry->data;
}


/*
 * @brief Internal function to get the component from entity
 * @param ecs  = Pointer to iso_ecs
 * @param ent  = iso_entity id
 * @param name = Name of the component
 * @return Returns pointer to the component data
 */

static void* __iso_entity_get_component(iso_ecs* ecs, iso_entity ent, char* name) {
	iso_assert(__iso_comp_table_search(ecs->table, name), "Component `%s` is not in component table.\n", name);

	iso_comp_record* rec = __iso_comp_table_get_record(ecs->table, name);
	iso_assert(iso_comp_record_search(rec, ent), "Entity `%d` doesnt have component `%s`.\n", ent, name);

	iso_comp_entry* entry = iso_comp_record_get_entry(rec, ent);
	iso_assert(entry, "Entity `%d` doesnt have component `%s`\n", ent, name);

	return entry->data;
}


/*
 * @brief Internal function to remove component from entity
 * @param ecs  = Pointer to iso_ecs
 * @param ent  = iso_entity id
 * @param name = Name of the component
 */

static void __iso_entity_remove_component(iso_ecs* ecs, iso_entity ent, char* name) {
	iso_assert(__iso_comp_table_search(ecs->table, name), "Component `%s` is not in table.\n", name);

	iso_comp_record* rec = __iso_comp_table_get_record(ecs->table, name);
	iso_assert(iso_comp_record_search(rec, ent), "Entity `%d` doesnt have component `%s`.\n", ent, name);

	iso_comp_record_remove_entry(rec, ent);
}


/*
 * @brief Macro to add a new component
 * @param ecs  = Pointer to iso_ecs
 * @param ent  = iso_entity id
 * @param comp = Component structure
 * @param ...  = Component parameters
 */

#define iso_entity_add_component(ecs, ent, comp, ...)   \
	({                                                    \
		comp c = { __VA_ARGS__ };                           \
		comp* cp = iso_alloc(sizeof(c));                    \
		memcpy(cp, &c, sizeof(c));                          \
		__iso_entity_add_component(ecs, ent, #comp, cp);    \
	})                                                    \


/*
 * @brief Macro to get a component
 * @param ecs  = Pointer to iso_ecs
 * @param ent  = iso_entity id
 * @param comp = Component structure
 */

#define iso_entity_get_component(ecs, ent, comp)\
	__iso_entity_get_component(ecs, ent, #comp)


/*
 * @brief Macro to remove a component
 * @param ecs  = Pointer to iso_ecs
 * @param ent  = iso_entity id
 * @param comp = Component structure
 */

#define iso_entity_remove_component(ecs, ent, comp)\
	__iso_entity_remove_component(ecs, ent, #comp)

#endif // __ISO_ECS_H__
