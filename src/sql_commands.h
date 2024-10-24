char	CREATE_STACK_COMMAND[] = "CREATE TABLE IF NOT EXISTS STACK("
								 "product_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
								 "name TEXT NOT NULL, "
								 "remaining INTEGER NOT NULL,"
								 "cost INTEGER NOT NULL,"
								 "revenue INTEGER,"
								 "profit INTEGER,"
								 "profit_procent REAL,"
								 "cost_1  INTEGER);";
								
char	CREATE_FINANCE_COMMAND[] = "CREATE TABLE IF NOT EXISTS FINANCE(" 
										 "month_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT," 
										 "name TEXT NOT NULL," 
										 "plan INTEGER NOT NULL," 
										 "real_profit INTEGER NOT NULL);";