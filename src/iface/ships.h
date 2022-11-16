#ifndef ships_h
#define ships_h

enum _tech_id {
	// ship main (0 - )
	HULL	=	0,			// general structural integrity, minor systems defense
	ENVIRONMENT_CTL,		// allows crew on ship to survive
	GENERATOR,				// generates and distributes power to systems
	ENGINE_SUBLIGHT,		// allows sub-lightspeed travel
};

typedef struct _shipmodule_t {
	uint8_t tech_id;
	uint8_t tech_class;
	char tech_name[13];
} shipmodule_t;

typedef struct _shipdata_t {
	uint8_t ship_module_count;
	uint8_t ship_module_max;
	shipmodule_t *modules;
} shipdata_t;

bool ship_load_data(const uint8_t* data, size_t len);

#endif
