using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Lab4.Models.ViewModels
{
    public class AdsViewModel
    {
        public IEnumerable<Ads> adImages { get; set; }
        public AdsViewModel GetMyEnumator()
        {
            return new AdsViewModel();
        }
    }
}
