--SELECT annual_inc, funded_amnt, funded_amnt_inv, installment, int_rate, loan_amnt, grade FROM loan WHERE grade BETWEEN 'A' AND 'C' LIMIT 10000;
--SELECT annual_inc, funded_amnt, funded_amnt_inv, installment, int_rate, loan_amnt, grade from loan WHERE grade BETWEEN 'A' AND 'C' LIMIT 10000 OFFSET 10000;
--SELECT MAX(id), MAX(annual_inc), MAX(funded_amnt), MAX(funded_amnt_inv), MAX(installment), MAX(int_rate), MAX(loan_amnt) FROM train_data
