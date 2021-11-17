#include <stdio.h>
#include "WearhouseManager.h"


Package *create_package(long priority, const char* destination){
	// TODO 3.1
	Package *package= (Package *)malloc(sizeof(Package));
	package -> destination = (char*)malloc(sizeof(char));
	package -> priority = priority;
	if(destination != NULL ){
		strcpy(package -> destination, destination);
	}
	else{
		package -> destination = NULL;}
	return package;
}

void destroy_package(Package* package){
	// TODO: 3.1
	if(package == NULL)
		return;
	while ( package -> destination != NULL){
			free(package -> destination);
			package -> destination = NULL;
		}
	free(package);
	package = NULL;
}

Manifest* create_manifest_node(void){
	// TODO: 3.1
	Manifest* node=(Manifest *)malloc(sizeof(Manifest));
	node -> next = NULL;
	node -> prev = NULL;
	return node;
}

void destroy_manifest_node(Manifest* manifest_node){
	// TODO: 3.1.
	if( manifest_node == NULL)
		return;
	destroy_manifest_node(manifest_node -> next);
	free(manifest_node);
}

Wearhouse* create_wearhouse(long capacity){
	// TODO: 3.2
	Wearhouse *wearhouse = (Wearhouse *)malloc(sizeof(Wearhouse));
	if( capacity == 0)
		return NULL;
	wearhouse -> size = 0;
	wearhouse -> capacity = capacity;
	wearhouse -> packages = (Package **)malloc(capacity *sizeof(Package **));
	for( long i=0; i< wearhouse -> size; i++)
		wearhouse -> packages[i] = NULL;
	return wearhouse;	
}
Wearhouse *open_wearhouse(const char* file_path){
	ssize_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Wearhouse *w = NULL;


	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	if((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		w = create_wearhouse(atol(token));

		free(line);
		line = NULL;
		len = 0;
	}

	while((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		long priority = atol(token);
		token = strtok(NULL, ",\n ");
		Package *p = create_package(priority, token);
		w->packages[w->size++] = p;

		free(line);
		line = NULL;
		len = 0;
	}

	free(line);


	fclose(fp);
	return w;

	file_open_exception:
	return NULL;
}

long wearhouse_is_empty(Wearhouse *w){
	// TODO: 3.2
	if( w -> size == 0)
		return 1;
	return 0;
}

long wearhouse_is_full(Wearhouse *w){
	// TODO: 3.2
	if ( w -> size == w -> capacity )
		return 1;
	return 0;
}

long wearhouse_max_package_priority(Wearhouse *w){
	// TODO: 3.2
	long i, maxim;
	maxim = w -> packages[0] -> priority;
	for(i = 1; i < w -> size; i++){
		if( w -> packages[i] -> priority > maxim)
			maxim = w -> packages[i] -> priority;
	}
	return maxim;
}

long wearhouse_min_package_priority(Wearhouse *w){
	// TODO: 3.2
	long i, minim;
	minim = w -> packages[0] -> priority;
	for(i = 1; i < w-> size; i++){
		if( w -> packages[i] -> priority < minim)
			minim = w -> packages[i] -> priority;
	}
	return minim;
}

void wearhouse_print_packages_info(Wearhouse *w){
	for(long i = 0; i < w->size; i++){
		printf("P: %ld %s\n",
				w->packages[i]->priority,
				w->packages[i]->destination);
	}
	printf("\n");
}

void destroy_wearhouse(Wearhouse* wearhouse){
	// TODO: 3.2
	if(wearhouse == NULL)
		return;
	if( wearhouse -> packages != NULL) {
		free( wearhouse -> packages);
		wearhouse -> packages = NULL;
	}
	free(wearhouse);
	wearhouse = NULL;
}


Robot* create_robot(long capacity){
	// TODO: 3.2
	Robot* robot=(Robot *)malloc(sizeof(Robot));
	robot -> size = 0;
	robot -> capacity = capacity;
	robot -> next = NULL;
	robot -> manifest = NULL;
	return robot;
}

int robot_is_full(Robot* robot){
	// TODO: 3.2
	if( robot -> size == robot -> capacity)
		return 1;
	return 0;
}

int robot_is_empty(Robot* robot){
	// TODO: 3.2
	if (robot -> size == 0 && robot -> manifest == NULL )
		return 1;
	return 0;
}

Package* robot_get_wearhouse_priority_package(Wearhouse *w, long priority){
	// TODO: 3.2
	long i;
	for(i = 0; i < w -> size; i++){
		if(w -> packages[i] -> priority == priority)
			return w -> packages[i];
	}
	return NULL;
}

void robot_remove_wearhouse_package(Wearhouse *w, Package* package){
	// TODO: 3.2
	long i, aux = w -> size - 1;
	while ( aux >= 0 && w -> packages[aux] != package ){
		aux--;
	}
	if(aux < 0)
		return;
	for(i = aux + 1 ; i < w -> size; i++){
		w -> packages[i -1] = w -> packages [i];
	}
	if(w -> size != 0)
		w -> size--;
}

void robot_load_one_package(Robot* robot, Package* package){
	// TODO:  3.2
	if ( robot == NULL || package == NULL)
		return;
	Manifest* node = (Manifest *)malloc(sizeof(Manifest));
	node -> package = package;
	node -> next = NULL;
	node -> prev = NULL;
	if(robot -> manifest == NULL){
		robot -> manifest =node;
		robot -> size++;
		return;
	}

	// Caut locul in care inserez pachetul
	Manifest* var = robot -> manifest;
	Manifest *ultim = NULL;
	while( var != NULL && var -> package -> priority > package -> priority){
		ultim = var;
		var = var -> next;
	}
	while( var != NULL && var -> package -> priority == package -> priority && strncmp(package -> destination, var -> package -> destination, MAX_DESTINATION_NAME_LEN) > 0){
		ultim = var;
		var = var -> next;
	}
	// Il inserez la inceputul liste
	if( var == robot -> manifest){
		node -> next = robot -> manifest;
		robot -> manifest -> prev = node;
		robot -> manifest  = node;
		node -> prev = NULL;
	}else if( var != robot -> manifest && var != NULL){			//Cazul general cand sunt undeva in lista
		node -> next = var;
		var -> prev = node;
		node -> prev = ultim;
		ultim -> next = node;
	}else if( var == NULL){       		//Il inserez la sfarsit
		node -> next = NULL;
		node -> prev = ultim;
		ultim -> next = node;
	}
	robot -> size++;
}

long robot_load_packages(Wearhouse* wearhouse, Robot* robot){
	// TODO: 3.2
	long nr= 0;
	while( !robot_is_full( robot) && !wearhouse_is_empty (wearhouse)) {
		long p = wearhouse_max_package_priority(wearhouse);   	//gasesc prioritatea maxima
		Package *pack= robot_get_wearhouse_priority_package(wearhouse, p);   //gasesc pachetul
		robot_load_one_package(robot, pack);				  //incarc pachetul in robot
		robot_remove_wearhouse_package(wearhouse, pack);		 //elimin pachetul din depozit
		nr++;
	}
	return nr;

}
Package* robot_get_destination_highest_priority_package(Robot* robot, const char* destination){
	// TODO: 3.2
	long max =0;
	Package *aux = NULL;
	Manifest* var = robot -> manifest;
	while ( var != NULL){
		if( var -> package -> priority > max && strncmp( destination, var -> package -> destination, MAX_DESTINATION_NAME_LEN) == 0) {
			max = var -> package -> priority;
			aux = var -> package;
		}
		var = var -> next;
	}
	return aux;
}
void destroy_robot(Robot* robot){
	// TODO: 3.2
	if(robot == NULL)
		return;
	if( robot -> manifest != NULL){
		free( robot -> manifest);
		robot -> manifest = NULL;
	}
	free(robot);
	robot = NULL;
}

void robot_unload_packages(Truck* truck, Robot* robot){
	// TODO: 3.3
	Manifest *urmator = NULL;
	Manifest *var = robot -> manifest;
	while( var != NULL && !truck_is_full(truck) && !robot_is_empty(robot)){
		if(strcmp( var -> package -> destination, truck -> destination) == 0){
			truck -> size++;
			robot -> size--;
			urmator = var -> next;
			if ( var -> next != NULL)
				var -> next -> prev = var -> prev;
			if ( var -> prev != NULL)
				var -> prev -> next = var -> next;
			else
				robot -> manifest = robot -> manifest -> next;
			if ( truck -> manifest == NULL){
				truck -> manifest = var;
				truck -> manifest -> next = NULL;
				truck -> manifest -> prev = NULL;
			}
			else {
				var -> next = truck -> manifest;
				truck -> manifest -> prev = var;
				truck -> manifest = var;
				var -> prev = NULL;
			}
			var = urmator;
		}else{
				var = var -> next;
			}
		}
}

// Attach to specific truck
int robot_attach_find_truck(Robot* robot, Parkinglot *parkinglot){
	int found_truck = 0;
	long size = 0;
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	Manifest* m_iterator = robot->manifest;


	while(m_iterator != NULL){
		while(arrived_iterator != parkinglot->arrived_trucks){
			size  = truck_destination_robots_unloading_size(arrived_iterator);
			if(strncmp(m_iterator->package->destination, arrived_iterator->destination, MAX_DESTINATION_NAME_LEN) == 0 &&
				size < (arrived_iterator->capacity-arrived_iterator->size)){
				found_truck = 1;
				break;
			}

			arrived_iterator = arrived_iterator->next;
		}

		if(found_truck)
			break;
		m_iterator = m_iterator->next;
	}

	if(found_truck == 0)
		return 0;


	Robot* prevr_iterator = NULL;
	Robot* r_iterator = arrived_iterator->unloading_robots;
	while(r_iterator != NULL){
		Package *pkg = robot_get_destination_highest_priority_package(r_iterator, m_iterator->package->destination);
		if(m_iterator->package->priority >= pkg->priority)
			break;
		prevr_iterator = r_iterator;
		r_iterator = r_iterator->next;
	}

	robot->next = r_iterator;
	if(prevr_iterator == NULL)
		arrived_iterator->unloading_robots = robot;
	else
		prevr_iterator->next = robot;

	return 1;
}

void robot_print_manifest_info(Robot* robot){
	Manifest *iterator = robot->manifest;
	while(iterator != NULL){
		printf(" R->P: %s %ld\n", iterator->package->destination, iterator->package->priority);
		iterator = iterator->next;
	}

	printf("\n");
}



Truck* create_truck(const char* destination, long capacity, long transit_time, long departure_time){
	// TODO: 3.3
	Truck* truck=(Truck *)malloc(sizeof(Truck));
	truck -> manifest = NULL;
	truck -> unloading_robots = NULL;
	truck -> destination =(char*)malloc(sizeof(char));
	if(destination != NULL ){
		strcpy(truck -> destination, destination);
	}
	else{
		truck -> destination = NULL;}
	truck -> size = 0;
	truck -> capacity = capacity;
	truck -> in_transit_time = 0;
	truck -> transit_end_time = transit_time;
	truck -> departure_time = departure_time;
	truck -> next = NULL;
	return truck;
}

int truck_is_full(Truck *truck){
	// TODO: 3.3
	if( truck -> size == truck -> capacity)
		return 1;
	return 0;
}

int truck_is_empty(Truck *truck){
	// TODO: 3.3
	if (truck -> size == 0 && truck -> manifest == NULL )
		return 1;
	return 0;
}

long truck_destination_robots_unloading_size(Truck* truck){
	// TODO: 3.3
	return 0;
}


void truck_print_info(Truck* truck){
	printf("T: %s %ld %ld %ld %ld %ld\n", truck->destination, truck->size, truck->capacity,
			truck->in_transit_time, truck->transit_end_time, truck->departure_time);

	Manifest* m_iterator = truck->manifest;
	while(m_iterator != NULL){
		printf(" T->P: %s %ld\n", m_iterator->package->destination, m_iterator->package->priority);
		m_iterator = m_iterator->next;
	}

	Robot* r_iterator = truck->unloading_robots;
	while(r_iterator != NULL){
		printf(" T->R: %ld %ld\n", r_iterator->size, r_iterator->capacity);
		robot_print_manifest_info(r_iterator);
		r_iterator = r_iterator->next;
	}
}


void destroy_truck(Truck* truck){
	// TODO: 3.3
	if(truck == NULL)
		return;
	if( truck -> manifest != NULL){
		free( truck -> manifest);
		truck -> manifest = NULL;
	}
	if( truck -> unloading_robots != NULL){
		free( truck -> unloading_robots);
		truck -> unloading_robots = NULL;
	}
	free(truck);
	truck = NULL;
}


Parkinglot* create_parkinglot(void){
	// TODO: 3.4
	// Allocate parking lot
	Parkinglot* parking = (Parkinglot *)malloc(sizeof(Parkinglot));
	parking -> arrived_trucks = (Truck *)malloc(sizeof(Truck));
	parking -> departed_trucks = (Truck *)malloc(sizeof(Truck));
	parking -> pending_robots = (Robot *)malloc(sizeof(Robot));
	parking -> standby_robots = (Robot *)malloc(sizeof(Robot));
	parking -> arrived_trucks -> next = parking -> arrived_trucks;
	parking -> departed_trucks -> next = parking -> departed_trucks;
	parking -> pending_robots -> next = parking -> pending_robots;
	parking -> standby_robots -> next = parking -> standby_robots;
	return parking;
}

Parkinglot* open_parckinglot(const char* file_path){
	ssize_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Parkinglot *parkinglot = create_parkinglot();

	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	while((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		// destination, capacitym transit_time, departure_time, arrived
		if(token[0] == 'T'){
			token = strtok(NULL, ",\n ");
			char *destination = token;

			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			token = strtok(NULL, ",\n ");
			long transit_time = atol(token);

			token = strtok(NULL, ",\n ");
			long departure_time = atol(token);

			token = strtok(NULL, ",\n ");
			int arrived = atoi(token);

			Truck *truck = create_truck(destination, capacity, transit_time, departure_time);

			if(arrived)
				truck_arrived(parkinglot, truck);
			else
				truck_departed(parkinglot, truck);

		}else if(token[0] == 'R'){
			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			Robot *robot = create_robot(capacity);
			parkinglot_add_robot(parkinglot, robot);

		}

		free(line);
		line = NULL;
		len = 0;
	}
	free(line);

	fclose(fp);
	return parkinglot;

	file_open_exception:
	return NULL;
}

void parkinglot_add_robot(Parkinglot* parkinglot, Robot *robot){
	// TODO: 3.4
	if( parkinglot == NULL)
		return;
	if( robot -> size  == 0)
	{
		if(parkinglot -> standby_robots -> next == parkinglot -> standby_robots){
			parkinglot -> standby_robots -> next = robot;
			robot -> next = parkinglot -> standby_robots;
			return;
		}
		Robot *r_prev = parkinglot -> standby_robots;
		Robot *r_in_use = parkinglot -> standby_robots -> next;
		while( r_in_use -> next != parkinglot -> standby_robots && robot -> capacity < r_in_use -> capacity){
			r_prev = r_in_use;
			r_in_use = r_in_use -> next;
		}
		r_prev -> next = robot;
		robot -> next = r_in_use;
		return;
	}
	else
		{if(parkinglot -> pending_robots -> next == parkinglot -> pending_robots){
			parkinglot -> pending_robots -> next = robot;
			robot -> next = parkinglot -> pending_robots;
			return;
		}
		Robot *r_prev = parkinglot -> pending_robots;
		Robot *r_in_use = parkinglot -> pending_robots -> next;
		while( r_in_use -> next != parkinglot -> pending_robots && robot -> capacity < r_in_use -> capacity ){
			r_prev = r_in_use;
			r_in_use = r_in_use -> next;
		}
		r_prev -> next = robot;
		robot -> next = r_in_use;
		return;
	}
}

void parkinglot_remove_robot(Parkinglot *parkinglot, Robot* robot){
	// TODO: 3.4
	if( parkinglot == NULL)
		return;
	Robot *var = parkinglot -> standby_robots;
	Robot *var2 = parkinglot -> pending_robots;
	if( robot -> size  == 0){
		while( var -> next != parkinglot -> standby_robots && var -> next != robot )
			var = var -> next;
		if( var -> next == robot){
			var -> next = var -> next -> next;
			robot -> next = NULL;
		}
	}
	else{
		while( var2 -> next != parkinglot -> pending_robots && var2 -> next != robot )
			var2 = var2 -> next;
		if( var2 -> next == robot){
			var2 -> next = var2 -> next -> next;
			robot -> next = NULL;
		}
	}
}

int parckinglot_are_robots_peding(Parkinglot* parkinglot){
	// TODO: 3.4
	if(parkinglot -> pending_robots -> next != parkinglot -> pending_robots)
		return 1;
	return 0;
}

int parkinglot_are_arrived_trucks_empty(Parkinglot* parkinglot){
	// TODO: 3.4
	if( parkinglot -> arrived_trucks -> next != parkinglot -> arrived_trucks)
		return 0;
	return 1;
}


int parkinglot_are_trucks_in_transit(Parkinglot* parkinglot){
	// TODO: 3.4
	if( parkinglot -> departed_trucks -> next != parkinglot -> departed_trucks)
		return 1;
	return 0;
}


void destroy_parkinglot(Parkinglot* parkinglot){
	// TODO: 3.4
	if(parkinglot == NULL)
		return;
	if( parkinglot -> arrived_trucks != NULL){
		free( parkinglot -> arrived_trucks );
		parkinglot -> arrived_trucks = NULL;
	}
	if( parkinglot -> departed_trucks != NULL){
		free( parkinglot -> departed_trucks );
		parkinglot -> departed_trucks = NULL;
	}
	if( parkinglot -> pending_robots != NULL){
		free( parkinglot -> pending_robots );
		parkinglot -> pending_robots = NULL;
	}
	if( parkinglot -> standby_robots != NULL){
		free( parkinglot -> standby_robots );
		parkinglot -> standby_robots = NULL;
	}
	free(parkinglot);
	parkinglot = NULL;
}

void parkinglot_print_arrived_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->arrived_trucks->next;
	while(iterator != parkinglot->arrived_trucks){

		truck_print_info(iterator);
		iterator = iterator->next;
	}

	printf("\n");

}

void parkinglot_print_departed_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->departed_trucks->next;
	while(iterator != parkinglot->departed_trucks){
		truck_print_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_pending_robots(Parkinglot* parkinglot){
	Robot *iterator = parkinglot->pending_robots->next;
	while(iterator != parkinglot->pending_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_standby_robots(Parkinglot* parkinglot){
	Robot *iterator = parkinglot->standby_robots->next;
	while(iterator != parkinglot->standby_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}


void truck_departed(Parkinglot *parkinglot, Truck* truck){
	// TODO: 3.5
	// Search through arrived list, if exists node is found remove it
	// Note: this must remove the node from the list, NOT deallocate it
}


void truck_arrived(Parkinglot *parkinglot, Truck* truck){
	if(parkinglot == NULL || truck == NULL) return;

	// TODO: 3.5
	// Search through departed list, if exists node is found remove it
	// Note: this must remove the node not deallocate it
}

void truck_transfer_unloading_robots(Parkinglot* parkinglot, Truck* truck){
	// TODO:  3.5
	Robot* robotel = truck -> unloading_robots;
	while( robotel != NULL){
		Robot* aux = robotel;
		robotel = robotel -> next;
		parkinglot_add_robot(parkinglot, aux);
	}
	truck -> unloading_robots = NULL;
}


// Depends on parking_turck_departed
void truck_update_depatures(Parkinglot* parkinglot, long day_hour){
	// TODO: 3.5
}

// Depends on parking_turck_arrived
void truck_update_transit_times(Parkinglot* parkinglot){
	// TODO: 3.5
}

void robot_swarm_collect(Wearhouse *wearhouse, Parkinglot *parkinglot){
	Robot *head_robot = parkinglot->standby_robots;
	Robot *current_robot = parkinglot->standby_robots->next;
	while(current_robot != parkinglot->standby_robots){

		// Load packages from wearhouse if possible
		if(!robot_load_packages(wearhouse, current_robot)){
			break;
		}

		// Remove robot from standby list
		Robot *aux = current_robot;
		head_robot->next = current_robot->next;
		current_robot = current_robot->next;

		// Add robot to the
		parkinglot_add_robot(parkinglot, aux);
	}
}


void robot_swarm_assign_to_trucks(Parkinglot *parkinglot){

	Robot *current_robot = parkinglot->pending_robots->next;

	while(current_robot != parkinglot->pending_robots){
		Robot* aux = current_robot;
		current_robot = current_robot->next;
		parkinglot_remove_robot(parkinglot, aux);
		int attach_succeded = robot_attach_find_truck(aux, parkinglot);
		if(!attach_succeded)
			parkinglot_add_robot(parkinglot, aux);
	}
}

void robot_swarm_deposit(Parkinglot* parkinglot){
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	while(arrived_iterator != parkinglot->arrived_trucks){
		Robot *current_robot = arrived_iterator->unloading_robots;
		while(current_robot != NULL){
			robot_unload_packages(arrived_iterator, current_robot);
			Robot *aux = current_robot;
			current_robot = current_robot->next;
			arrived_iterator->unloading_robots = current_robot;
			parkinglot_add_robot(parkinglot, aux);
		}
		arrived_iterator = arrived_iterator->next;
	}
}

