/*!
 * \file   ExternalFunctionExpr.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#include<sstream>
#include<algorithm>

#include"TFEL/Math/Parser/Number.hxx"
#include"TFEL/Math/Parser/Variable.hxx"
#include"TFEL/Math/Parser/BinaryOperator.hxx"
#include"TFEL/Math/Parser/ExternalFunctionExpr.hxx"
#include"TFEL/Math/Parser/ExternalFunctionExpr2.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {
      
      ExternalFunctionExpr::ExternalFunctionExpr(const std::string& fname,
						 std::vector<tfel::utilities::shared_ptr<Expr> >& fargs,
						 tfel::utilities::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m)
	: name(fname),
	  args(fargs),
	  manager(m)
      {} // end of ExternalFunctionExpr::ExternalFunctionExpr
      
      double
      ExternalFunctionExpr::getValue(void) const
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math::parser;
	vector<shared_ptr<Expr> >::const_iterator p;
	vector<shared_ptr<Expr> >::size_type i;
	ExternalFunctionManager::iterator p2;
	p2=this->manager->find(this->name);
	if(p2==this->manager->end()){
	  string msg("ExternalFunctionExpr::getValue : ");
	  msg += "unknown function '"+this->name+"'";
	  throw(runtime_error(msg));
	}
	if(p2->second->getNumberOfVariables()!=this->args.size()){
	  ostringstream msg;
	  msg << "ExternalFunctionExpr::getValue : "
	      << "invalid number of arguments for function '"
	      << this->name << "' (" 
	      << this->args.size() << " given, "
	      << p2->second->getNumberOfVariables() << " required)";
	  throw(runtime_error(msg.str()));
	}
	for(p=this->args.begin(),i=0u;p!=this->args.end();++p,++i){
	  const double val = (*p)->getValue();
	  p2->second->setVariableValue(i,val);
	}
	return p2->second->getValue();
      } // end of ExternalFunctionExpr::getValue

      void
      ExternalFunctionExpr::checkCyclicDependency(std::vector<std::string>& names) const
      {
	using namespace std;
	using namespace tfel::utilities;
	using std::vector;
	vector<string> pnames;
	vector<shared_ptr<Expr> >::const_iterator p;
	ExternalFunctionManager::const_iterator p2;
	if(find(names.begin(),names.end(),this->name)!=names.end()){
	  string msg("ExternalFunctionExpr::checkCyclicDependency : ");
	  msg += "cyclic dependency found on function '"+this->name+"'";
	  throw(runtime_error(msg));
	}
	p2=this->manager->find(this->name);
	if(p2==this->manager->end()){
	  string msg("ExternalFunctionExpr::checkCyclicDependency : ");
	  msg += "unknown function '"+this->name+"'";
	  throw(runtime_error(msg));
	}
	pnames = names;
	names.push_back(this->name);
	p2->second->checkCyclicDependency(names);
	for(p=this->args.begin();p!=this->args.end();++p){
	  vector<string> n(pnames);
	  (*p)->checkCyclicDependency(n);
	  mergeVariablesNames(names,n);
	}
      } // end of ExternalFunctionExpr::checkCyclicDependency

      tfel::utilities::shared_ptr<Expr>
      ExternalFunctionExpr::differentiate(const std::vector<double>::size_type pos,
					  const std::vector<double>& v) const
      {
	using namespace std;
	using namespace tfel::utilities;
	using std::vector;
	vector<shared_ptr<Expr> > nargs(this->args.size());
	vector<shared_ptr<Expr> >::const_iterator p = this->args.begin();
	ExternalFunctionManager::const_iterator p2;
        vector<shared_ptr<Expr> >::const_iterator p3;
        vector<shared_ptr<Expr> >::iterator p4;
	unsigned short i = 0;
	if(args.size()==0){
	  return shared_ptr<Expr>(new Number(0.));
	}
	p2=this->manager->find(this->name);
	if(p2==this->manager->end()){
	  string msg("ExternalFunctionExpr::differentiate : ");
	  msg += "unknown function '"+this->name+"'";
	  throw(runtime_error(msg));
	}
	if(p2->second->getNumberOfVariables()!=this->args.size()){
	  ostringstream msg;
	  msg << "ExternalFunctionExpr::getValue : "
	      << "invalid number of arguments for function '"
	      << this->name << "' (" 
	      << this->args.size() << " given, "
	      << p2->second->getNumberOfVariables() << " required)";
	  throw(runtime_error(msg.str()));
	}
        for(p3=this->args.begin(),p4=nargs.begin();
	    p3!=this->args.end();++p3,++p4){
	  *p4 = (*p3)->clone(v);
	}
        shared_ptr<Expr> df_(new ExternalFunctionExpr2(p2->second->differentiate(i),
						     nargs));
        shared_ptr<Expr> df = shared_ptr<Expr>(new BinaryOperation<OpMult>(df_,
								       (*p)->differentiate(pos,v)));
        ++p;
        ++i;
        while(p!=this->args.end()){
	  df_  = shared_ptr<Expr>(new ExternalFunctionExpr2(p2->second->differentiate(i),
							  nargs));
	  shared_ptr<Expr> df2 = shared_ptr<Expr>(new BinaryOperation<OpMult>(df_,
									  (*p)->differentiate(pos,v)));
      
          df = shared_ptr<Expr>(new BinaryOperation<OpPlus>(df,df2));
	  ++p;
	  ++i;
        }
	return df;
      } // end of ExternalFunctionExpr::differentiate

      tfel::utilities::shared_ptr<Expr>
      ExternalFunctionExpr::clone(const std::vector<double>& v) const
      {
	using namespace std;
	using namespace tfel::utilities;
	using std::vector;
	vector<shared_ptr<Expr> > nargs(this->args.size());
        vector<shared_ptr<Expr> >::const_iterator p;
        vector<shared_ptr<Expr> >::iterator p2;
        for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->clone(v);
	}
        return shared_ptr<Expr>(new ExternalFunctionExpr(this->name,nargs,this->manager));	
      } // end of ExternalFunctionExpr::clone

      tfel::utilities::shared_ptr<Expr>
      ExternalFunctionExpr::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v,
									    const std::vector<std::string>& params,
									    const std::map<std::string,
									    std::vector<double>::size_type>& pos) const
      {
	using namespace std;
	using namespace tfel::utilities;
        vector<shared_ptr<Expr> >::const_iterator p;
        vector<shared_ptr<Expr> >::iterator p2;
	map<string,vector<double>::size_type>::const_iterator p3;
	vector<shared_ptr<Expr> > nargs;
	vector<string> vnames;
	vector<string>::size_type i;
	tfel::utilities::shared_ptr<ExternalFunction> nf;
	ExternalFunctionManager::iterator pf;
	if(this->args.size()==0){
	  if(find(params.begin(),params.end(),this->name)!=params.end()){
	    p3 = pos.find(this->name);
	    if(p3==pos.end()){
	      string msg("ExternalFunctionExpr::createFunctionByChangingParametersIntoVariables : ");
	      msg += "internal error (no position found for parameter '"+this->name+"')";
	      throw(runtime_error(msg));
	    }
	    shared_ptr<Expr> nv = shared_ptr<Expr>(new Variable(v,p3->second));
	    return nv;
	  }
	}
	pf = this->manager->find(this->name);
	if(pf==this->manager->end()){
	  string msg("ExternalFunctionExpr::createFunctionByChangingParametersIntoVariables : ");
	  msg += "no function '"+this->name+"' declared";
	  throw(runtime_error(msg));
	}
	if(pf->second->getNumberOfVariables()!=this->args.size()){
	  ostringstream msg;
	  msg << "ExternalFunctionExpr::getValue : "
	      << "invalid number of arguments for function '"
	      << this->name << "' (" 
	      << this->args.size() << " given, "
	      << pf->second->getNumberOfVariables() << " required)";
	  throw(runtime_error(msg.str()));
	}
	nf = pf->second->createFunctionByChangingParametersIntoVariables(vnames,v,params,pos);
	if(nf->getNumberOfVariables()<this->args.size()){
	  string msg;
	  msg += "ExternalFunctionExpr::getValue : "
	    "internal error (function as less variable after "
	    "'createFunctionByChangingParametersIntoVariables' than before";
	  throw(runtime_error(msg));
	}
	nargs.resize(nf->getNumberOfVariables());
	for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->createFunctionByChangingParametersIntoVariables(v,params,pos);
	}
	for(i=0;i!=vnames.size();++i){
	  p3 = pos.find(vnames[i]);
	  if(p3==pos.end()){
	    string msg("ExternalFunctionExpr::createFunctionByChangingParametersIntoVariables : ");
	    msg += "internal error (no position found for parameter '"+vnames[i]+"')";
	    throw(runtime_error(msg));
	  }
	  nargs[args.size()+i] = shared_ptr<Expr>(new Variable(v,p3->second));
	}
        return shared_ptr<Expr>(new ExternalFunctionExpr2(nf,nargs));
      } // end of ExternalFunctionExpr::createFunctionByChangingParametersIntoVariables

      void
      ExternalFunctionExpr::getParametersNames(std::set<std::string>& p) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<shared_ptr<Expr> >::const_iterator pa;
	ExternalFunctionManager::iterator pf;
	if(this->args.size()==0){
	  p.insert(this->name);
	} else {
	  pf = this->manager->find(this->name);
	  if(pf==this->manager->end()){
	    string msg("ExternalFunctionExpr::getParametersNames : ");
	    msg += "no function '"+this->name+"' declared";
	    throw(runtime_error(msg));
	  }
	  pf->second->getParametersNames(p);
	  for(pa=this->args.begin();pa!=this->args.end();++pa){
	    (*pa)->getParametersNames(p);
	  }
	}
      } // end of ExternalFunctionExpr::getParametersNames(std::set<std::string>&) const;

      tfel::utilities::shared_ptr<Expr>
      ExternalFunctionExpr::resolveDependencies(const std::vector<double>& v) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<shared_ptr<Expr> > nargs(this->args.size());
        vector<shared_ptr<Expr> >::const_iterator p;
        vector<shared_ptr<Expr> >::iterator p2;
	ExternalFunctionManager::iterator p3;
        for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->resolveDependencies(v);
	}
	p3=this->manager->find(this->name);
	if(p3==this->manager->end()){
	  string msg("ExternalFunctionExpr::getValue : ");
	  msg += "unknown function '"+this->name+"'";
	  throw(runtime_error(msg));
	}
	if(p3->second->getNumberOfVariables()!=this->args.size()){
	  ostringstream msg;
	  msg << "ExternalFunctionExpr::getValue : "
	      << "invalid number of arguments for function '"
	      << this->name << "' (" 
	      << this->args.size() << " given, "
	      << p3->second->getNumberOfVariables() << " required)";
	  throw(runtime_error(msg.str()));
	}
        return shared_ptr<Expr>(new ExternalFunctionExpr2(p3->second->resolveDependencies(),nargs));	
      } // end of ExternalFunctionExpr::resolveDependencies
      
    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
